/// Use this file with videos (images.cpp is for images) ///

#include "detection.h"

int main(int argc, char* argv[])
{   
    // Load source video
    if (argc != 2) {
        std::cout << "Usage: ./exe path-to-video" << std::endl;
        return -1;
    }

    // Initialize video capture for reading a videofile
    VideoCapture cap(argv[1]);

    // Check if video can be opened
    if (!cap.isOpened())
    {
        std::cout << "Failed to open videofile!" << std::endl;
        return -1;
    }
    
    // Read and analyze video
    while (true)
    {
        Mat frame;
        cap >> frame;

        // Stop if frame is empty (end of video)
        if (frame.empty())
        {
            break;
        }

        // Filter image 
        Mat filteredIMG = filterColors(frame);

        // Apply grayscale
        Mat gray = applyGrayscale(filteredIMG);

        // Apply Gaussian blur
        Mat gBlur = applyGaussianBlur(gray);

        // Find edges
        Mat edges = applyCanny(gBlur);

        // Create mask (Region of Interest)
        Mat polyMask = RegionOfInterest(edges);

        // Draw the lanes
        std::vector<Vec4i> linesP = houghLines(polyMask);
        Mat lanes = drawLanes(frame.clone(), linesP);
        imshow("Lanes", lanes);

        // Press  ESC on keyboard to exit
        if (waitKey(5) == 27) break;
    }

    cap.release();
   
    return 0;
}