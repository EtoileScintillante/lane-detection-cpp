/// Use this file with images (main.cpp is for video) ///

#include "detection.h"

int main(int argc, char* argv[])
{   
    /// Load source image ///
    if (argc != 2) {
        std::cout << "Usage: ./exe path-to-image" << std::endl;
    }
    std::string path = argv[1];
    Mat source = imread(path, 1);
    imshow("Source", source);
    waitKey(0);

    Mat filteredIMG = filterColors(source);
    imshow("Color filtering", filteredIMG);
    waitKey(0);

    Mat gray = applyGrayscale(filteredIMG);

    Mat gBlur = applyGaussianBlur(gray);
    imshow("Gray scale + Gaussian blur", gBlur);
    waitKey(0);

    Mat edges = applyCanny(gBlur);
    imshow("Canny edge detection", edges);
    waitKey(0);

    Mat sourceMask = RegionOfInterest(source.clone());
    imshow("Masked source", sourceMask);
    waitKey(0);

    Mat polyMask = RegionOfInterest(edges);
    imshow("Masked image", polyMask);
    waitKey(0);

    std::vector<Vec4i> linesP = houghLines(polyMask);
    Mat lanes = drawLanes(source.clone(), linesP);
    imshow("Lanes", lanes);
    waitKey(0);

    return 0;
} 