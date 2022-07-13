#include "detection.h"

Mat applyGrayscale(Mat source)
{
    Mat dst;
    cvtColor(source, dst, COLOR_BGR2GRAY);

    return dst;
}

Mat applyGaussianBlur(Mat source)
{
    Mat dst;
    GaussianBlur(source, dst, Size(3, 3), 0);

    return dst;
}

Mat applyCanny(Mat source)
{
    Mat dst;
    Canny(source, dst, 50, 150);

    return dst;
}

Mat filterColors(Mat source)
{
    Mat hsv, whiteMask, whiteImage, yellowMask, yellowImage, dst;

    // White mask
    std::vector< int > lowerWhite = {200, 200, 200};
    std::vector< int > upperWhite = {255, 255, 255};
    inRange(source, lowerWhite, upperWhite, whiteMask);
    bitwise_and(source, source, whiteImage, whiteMask);

    // Yellow mask
    cvtColor(source, hsv, COLOR_BGR2HSV);
    std::vector< int > lowerYellow = {20, 80, 70}; 
    std::vector< int > upperYellow = {110, 255, 255};
    inRange(hsv, lowerYellow, upperYellow, yellowMask);
    bitwise_and(source, source, yellowImage, yellowMask);

    // Blend them together
    addWeighted(whiteImage, 1., yellowImage, 1., 0., dst);

    return dst;
}

Mat RegionOfInterest(Mat source)
{
    // We want a trapezoid shape where the bottom edge is positioned at the bottom of the image
    float trapezoidBottomWidth = 0.85; // Width of bottom edge of trapezoid, expressed as percentage of image width
    float trapezoidTopWidth = 0.07; // Above comment also applies here, but then for the top edge of trapezoid
    float trapezoidHeight = 0.4; // Height of the trapezoid expressed as percentage of image height
    
    // The four points that form the polygon
    Point pts[4] = 
    {
        cv::Point((source.cols * (1 - trapezoidBottomWidth)) / 2, source.rows), // Bottom left point
        cv::Point((source.cols * (1 - trapezoidTopWidth)) / 2, source.rows - source.rows * trapezoidHeight), // Top left point 
        cv::Point(source.cols - (source.cols * (1 - trapezoidTopWidth)) / 2, source.rows - source.rows * trapezoidHeight), // Top right point
        cv::Point(source.cols - (source.cols * (1 - trapezoidBottomWidth)) / 2, source.rows) // Bottom right point
    };
    
    // Mask is same size as original image
    Mat mask = Mat::zeros(source.size(), source.type());

    // Here we fill the trapezoid with the specified color (white)
    fillConvexPoly(mask, pts, 4, Scalar(255, 255, 255));

    /* And here we basically put the mask over the source image,
    meaning we return an all black image, except for the part where the mask image
    has nonzero pixels: all the white pixels inside trapezoid */
    Mat maskedImage;
    bitwise_and(source, mask, maskedImage);

    return maskedImage;
}

Mat drawLanes(Mat source, std::vector<Vec4i> lines)
{
    // Stop if there are no lines, just return original image without lines
    if (lines.size() == 0)
    {
        return source;
    }

    // Set drawing lanes to true
    bool drawRightLane = true;
    bool drawLeftLane = true;

    // Find lines with a slope higher than the slope threshold
    float slopeThreshold = 0.5;
    std::vector< float > slopes;
    std::vector< Vec4i > goodLines;

    for (int i = 0; i < lines.size(); i++)
    {
        /* Each line is represented by a 4-element vector (x_1, y_1, x_2, y_2), 
        where (x_1,y_1) is the line's starting point and (x_2, y_2) is the ending point */
        Vec4i l = lines[i];
        double slope;

        // Calculate slope
        if (l[2] - l[0] == 0) // Avoid division by zero
        {
            slope = 999; // Basically infinte slope
        }
        else 
        {
            slope = (l[3] - l[1]) / (l[2] / l[0]);
        }
        if (abs(slope) > 0.5)
        {
            slopes.push_back(slope);
            goodLines.push_back(l);
        }
    }

    /* Split the good lines into two categories: right and left
    The right lines have a positive slope and the left lines have a negative slope */
    std::vector< Vec4i > rightLines;
    std::vector< Vec4i > leftLines;
    int imgCenter = source.cols / 2;

    for (int i = 0; i < slopes.size(); i++)
    {
        if (slopes[i] > 0 && goodLines[i][0] > imgCenter && goodLines[i][2] > imgCenter)
        {
            rightLines.push_back(goodLines[i]);
        }
        if (slopes[i] < 0 && goodLines[i][0] < imgCenter && goodLines[i][2] < imgCenter)
        {
            leftLines.push_back(goodLines[i]);
        }
    }

    /* Now that we've isolated the right lane lines from the left lane lines,
    it is time to form two lane lines out of all the lines we've detected.
    A line is defined as 2 points: a starting point and an ending point.
    So up to this point the right and left lane basically consist of multiple hough lines.
    Our goal at this step is to use linear regression to find the two best fitting lines:
    one through the points at the left side to form the left lane
    and one through the points at the right side to form the right lane */

    // We start with the right side points
    std::vector< int > rightLinesX;
    std::vector< int > rightLinesY;
    double rightB1, rightB0;

    for (int i = 0; i < rightLines.size(); i++)
    {
        rightLinesX.push_back(rightLines[i][0]); // X of starting point of line
        rightLinesX.push_back(rightLines[i][2]); // X of ending point of line
        rightLinesY.push_back(rightLines[i][1]); // Y of starting point of line
        rightLinesY.push_back(rightLines[i][3]); // Y of ending point of line
    }

    if (rightLinesX.size() > 0)
    {
        std::vector< double > coefRight = estimateCoefficients(rightLinesX, rightLinesY); // y = b1x + b0
        rightB1 = coefRight[0];
        rightB0 = coefRight[1];
    }
    else 
    {
        rightB1 = 1;
        rightB0 = 1;
        drawRightLane = false;
    }

    // Now the points at the left side
    std::vector< int > leftLinesX;
    std::vector< int > leftLinesY;
    double leftB1, leftB0;

    for (int i = 0; i < leftLines.size(); i++)
    {
        leftLinesX.push_back(leftLines[i][0]); // X of starting point of line
        leftLinesX.push_back(leftLines[i][2]); // X of ending point of line
        leftLinesY.push_back(leftLines[i][1]); // Y of starting point of line
        leftLinesY.push_back(leftLines[i][3]); // Y of ending point of line
    }

    if (leftLinesX.size() > 0)
    {
        std::vector< double > coefLeft = estimateCoefficients(leftLinesX, leftLinesY); // y = b1x + b0
        leftB1 = coefLeft[0];
        leftB0 = coefLeft[1];
    }
    else 
    {
        leftB1 = 1;
        leftB0 = 1;
        drawLeftLane = false;
    }

    // Now we need to find the two points for the right and left lane
    int y1 = source.rows;
    /* 0.4 = trapezoidHeight (see RegionOfInterest), we set the y coordinate of the ending point
    below the trapezoid height (0.35) to draw shorter lanes. I think that looks nicer. */
    int y2 = source.rows * (1 - 0.35); 

    // y = b1x + b0 --> x = (y - b0) / b1
    int rightX1 = (y1 - rightB0) / rightB1;
    int rightX2 = (y2 - rightB0) / rightB1;

    int leftX1 = (y1 - leftB0) / leftB1;
    int leftX2 = (y2 - leftB0) / leftB1;

    // Create the mask
    Mat mask = Mat::zeros(source.size(), source.type());

    // Draw the lanes on mask
    if (drawRightLane == true)
    {
        line(source, Point(rightX1, y1), Point(rightX2, y2), Scalar(255, 0, 0), 7);
    }
    if (drawLeftLane == true)
    {
        line(source, Point(leftX1, y1), Point(leftX2, y2), Scalar(255, 0, 0), 7);
    }

    // Fill poly made up from the four points described above if both lanes are drawn
    Mat dst; // Holds blended image
    if (drawRightLane == true && drawLeftLane == true)
    {
        Point pts[4] = {
        Point(leftX1, y1), // Starting point left lane
        Point(leftX2, y2), // Ending point left lane
        Point(rightX2, y2), // Ending point right lane
        Point(rightX1, y1) // Starting point right lane
        };
        fillConvexPoly(mask, pts, 4, Scalar(235, 229, 52)); // Color is light blue

        // Blend the mask and source image together
        addWeighted(source, 0.7, mask, 0.3, 0.0, dst);
        return dst; // Return blended image
    }
    return source; // Return source if drawing lanes did not happen
}

std::vector<Vec4i> houghLines(Mat canny)
{
    double rho = 2; // Distance resolution in pixels of the Hough grid
    double theta = 1 * M_PI / 180; // Angular resolution in radians of the Hough grid
    int thresh = 15; // Minimum number of votes (intersections in Hough grid cell)
    double minLineLength = 10; // Minimum number of pixels making up a line
    double maxLineLength = 20; // Maximum gap in pixels between connectable line segments
   
    std::vector<Vec4i> linesP; // Will hold the results of the detection
    HoughLinesP(canny, linesP, rho, theta, thresh, minLineLength, maxLineLength);

    return linesP;
}