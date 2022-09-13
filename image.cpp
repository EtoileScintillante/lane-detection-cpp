/// Use this file with images ///

#include "lane-detection.h"

int main(int argc, char* argv[])
{   
    // Load source image
    if (argc != 2) {
        std::cout << "Usage: ./exe path-to-image" << std::endl;
        return 1;
    }
    std::string path = argv[1];
    Mat source = imread(path, 1);
    imshow("Source", source);
    waitKey(0);

    // Determine if image is taken during the day or not
    bool dayTime = isDayTime(source);

    // Show source with mask
    Mat maskedS = RegionOfInterest(source);
    imshow("Region of Interest on source", maskedS);
    waitKey(0);

    // Filter colors
    Mat filteredIMG = filterColors(source.clone(), dayTime);
    imshow("Color filtering", filteredIMG);
    waitKey(0);
    //imwrite("colorFilter.png", filteredIMG);
    
    // Apply grayscale
    Mat gray = applyGrayscale(filteredIMG);

    // Blur image
    Mat gBlur = applyGaussianBlur(gray);
    imshow("Grayscale and blur", gBlur);
    //imwrite("grayBlur.png", gBlur);

    // Detect edges
    Mat edges = applyCanny(gray);
    imshow("Canny edge detection", edges);
    waitKey(0);
    //imwrite("canny.png", edges);

    // Apply mask on canny
    Mat maskedIMG = RegionOfInterest(edges);
    imshow("Region of Interest on canny", maskedIMG);
    waitKey(0);
    //imwrite("maskedCanny.png", maskedIMG);

    // Detect lines and draw lanes if possible
    std::vector<Vec4i> linesP = houghLines(maskedIMG, source.clone(), false);
    Mat lanes = drawLanes(source.clone(), linesP);
    imshow("Lanes", lanes);
    waitKey(0);
    //imwrite("result.png", lanes);
    
    return 0;
} 