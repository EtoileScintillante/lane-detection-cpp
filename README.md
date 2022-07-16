# lane-detection-cpp

Inspired by [this](https://github.com/georgesung/road_lane_line_detection) python lane detection program.

## Example
<img src="https://github.com/EtoileScintillante/lane-detection-cpp/blob/master/docs/result.gif" width="450" alt="result video"/>

## Compile 
   ```
cmake . && make
```
## Run
   ```
./Runner path-to-video
```
## Requirements
* [CMake](https://cmake.org/download/) 3.2 or higher 
* [OpenCV](https://github.com/opencv/opencv)

## How it works
1. Filter out pixels that are not yellow or white (or gray if video is taken during the night/evening).
   
   <img src="https://github.com/EtoileScintillante/lane-detection-cpp/blob/master/docs/colorFilter.png" width="450" alt="color filter"/>
2. Convert image to grayscale and apply Gaussian blur. This is an important step since it reduces noise, and the algorithm for detecting edges (Canny Edge Detection) is susceptible to noise.

   <img src="https://github.com/EtoileScintillante/lane-detection-cpp/blob/master/docs/grayBlur.png" width="450" alt="grascale and blur"/>
3. Use the [Canny Edge Detection algorithm](https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html) to detect edges. We're interested in the edges of the lanes.

   <img src="https://github.com/EtoileScintillante/lane-detection-cpp/blob/master/docs/canny.png" width="450" alt="canny edges"/>
4. Find the region of interest. For this program that means creating a mask with the use of two trapezoids (a big one and a smaller one that goes inside the big one). We get the following result after applying the mask on the canny image:

   <img src="https://github.com/EtoileScintillante/lane-detection-cpp/blob/master/docs/maskedCanny.png" width="450" alt="trapezoid mask"/>
5. Detect lines using the [Hough Line Transform](https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html). HLT is used to detect straight lines. Every detected line is made up of two points (starting and ending point). Here are the detected lines drawn:

   <img src="https://github.com/EtoileScintillante/lane-detection-cpp/blob/master/docs/houghLines.png" width="450" alt="hough lines"/>
5. As you can see in the picture above, the right and left lane consist of multiple detected lines and as said before, a line is just two points. Our goal is to form two lines from those points, one line for the right lane and one for the left lane. This brings us to the fun part: using linear regression to find the line of best fit through the points. To do that, we first filter out the points that form a line with a slope below a certain threshold, then we isolate the left points from the right points and finally we use linear regression to obtain two well-fitting lines. And to make it a bit more clear, we fill in the space between the lines with a lighter color. Result: 

   <img src="https://github.com/EtoileScintillante/lane-detection-cpp/blob/master/docs/result.png" width="450" alt="lanes drawn"/>

## Limitations
This program is very basic.
It can only be used for detecting straight (and slightly curved) lanes. 
Furthermore, the dashcam videos and images used while creating this program all have a size of 1280x960
and they were all taken from the same perspective (the built-in front camera of my car).
If you want to work with videos/images taken from a different perspective and/or with a different size,
the parameters used for calculating the region of interest most likely have to be changed.
These are probably the biggest limitations. 


