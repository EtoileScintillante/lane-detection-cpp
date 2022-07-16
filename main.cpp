/// Use this file with videos ///

#include "lane-detection.h"

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
    // get one frame from camera to know frame size and type
    Mat src;
    cap >> src;
    // check if we succeeded
    if (src.empty()) {
        std::cout << "ERROR! blank frame grabbed\n";
        return -1;
    }
    bool isColor = (src.type() == CV_8UC3);
    //--- INITIALIZE VIDEOWRITER
    VideoWriter writer;
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    double fps = 25.0;                          // framerate of the created video stream
    std::string filename = "resultDC5.avi";             // name of the output video file
    writer.open(filename, codec, fps, src.size(), isColor);
    // check if we succeeded
    if (!writer.isOpened()) {
        std::cout << "Could not open the output video file for write\n";
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

        // Determine if video is taken during daytime or not
        bool isDay = isDayTime(frame);

        // Filter image 
        Mat filteredIMG = filterColors(frame, isDay);

        // Apply grayscale
        Mat gray = applyGrayscale(filteredIMG);

        // Apply Gaussian blur
        Mat gBlur = applyGaussianBlur(gray);

        // Find edges
        Mat edges = applyCanny(gBlur);

        // Create mask (Region of Interest)
        Mat maskedIMG = RegionOfInterest(edges);

        // Detect straight lines and draw the lanes if possible
        std::vector<Vec4i> linesP = houghLines(maskedIMG, frame.clone(), false);
        Mat lanes = drawLanes(frame, linesP);
        imshow("Lanes", lanes);

        writer.write(lanes);

        // Press  ESC on keyboard to exit
        if (waitKey(5) == 27) break;
    }

    cap.release();
   
    return 0;
}