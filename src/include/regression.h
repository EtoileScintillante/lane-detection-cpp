/*                                  
 * regression.h
 *
 * Created on: 11 July 2022
 * Author: EtoileScintillante
 */

#ifndef __REGRESSION__
#define __REGRESSION__

#include <vector>
#include <numeric>
#include <opencv2/opencv.hpp>

/**
 * @brief Multiplies two vectors and then calculates the sum of the multiplied values.
 * a and b must be the same size.
 * This function helps with the calculation of the cross-deviation and deviation about x.
 * 
 * @param a vector<int>
 * @param b vector<int>
 * @see estimateCoefficients
 * @return int sum
 */
int multiplyAndSum(std::vector<int> a, std::vector<int> b);

/**
 * @brief Calculates the coefficients (slope and intercept) of the best fitting line
 * given X and Y values.
 * 
 * @param X vector<int> X values (independent variables)
 * @param Y vector<int> Y values (dependent values)
 * @return std::vector<double> where first element is the slope (b1), second element is intercept (b0)
 */
std::vector<double> estimateCoefficients(std::vector<int> X, std::vector<int> Y);

#endif /*__REGRESSION__*/