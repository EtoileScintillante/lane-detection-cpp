/*                                  
 * detection.h
 *
 * Created on: 10 July 2022
 * Author: EtoileScintillante
 */

#ifndef __DETECTION__
#define __DETECTION__

#include <cmath>
#include "regression.h"
using namespace cv;

/**
 * @brief Apply grayscale transform on image.
 * 
 * @param source image that needs to be transformed
 * @return grayscale image
 */
Mat applyGrayscale(Mat source);

/**
 * @brief Apply Gaussian blur to image.
 * 
 * @param source image that needs to be blurred
 * @return blurred image
 */
Mat applyGaussianBlur(Mat source);

/**
 * @brief Detect edges of image by applying canny edge detection.
 * 
 * @param source image of which the edges needs to be detected
 * @return image with detected edges
 */
Mat applyCanny(Mat source);

/**
 * @brief Filter source image so that only the white and yellow pixels remain.
 * One assumption for lane detection here is that lanes are either white or yellow.
 * 
 * @param source source image
 * @return Mat filtered image
 */
Mat filterColors(Mat source);

/**
 * @brief Apply an image mask. 
 * Only keep the part of the image defined by the
 * polygon formed from four points. The rest of the image is set to black. 
 * 
 * @param source image on which to apply the mask
 * @return Mat image with mask
 */
Mat RegionOfInterest(Mat source);

/**
 * @brief Creates mask and blends it with source image so that the lanes
 * are drawn on the source image.
 * 
 * @param source source image
 * @param lines vector < vec4i > holding the lines
 * @return Mat image with lines drawn on it
 */
Mat drawLanes(Mat source, std::vector<Vec4i> lines);

/**
 * @brief Returns a vector with the detected hough lines.
 * 
 * @param canny image resulted from a canny transform
 * @see applyCanny
 * @return vector<Vec4i> contains hough lines.
 */
std::vector<Vec4i> houghLines(Mat canny);

#endif /*__DETECTION__*/