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
 * vector A and B must be the same size and their values must be of the same type.
 * 
 * @param A vector<T>.
 * @param B vector<T>.
 * @return X sum of the multiplied values.
 */
template <typename T, typename X>
X multiplyAndSum(std::vector<T> A, std::vector<T> B)
{
    X sum;
    std::vector<T> temp;
    for (int i = 0; i < A.size(); i++)
    {
        temp.push_back(A[i]*B[i]);
    }
    sum = std::accumulate(temp.begin(), temp.end(), 0.0);

    return sum;
}

/**
 * @brief Calculates the coefficients (slope and intercept) of the best fitting line
 * given independent and dependent values. Vector A and B must be the same size 
 * and their values must be of the same type.
 * 
 * @param A vector<T> (independent values).
 * @param B vector<T> (dependent values).
 * @return vector<X> where first element is the slope (b1), second element is intercept (b0).
 */
template <typename T, typename X>
std::vector< X > estimateCoefficients(std::vector<T> A, std::vector<T> B)
{
    // Sample size
    int N = A.size();

    // Calculate mean of X and Y
    X meanA= std::accumulate(A.begin(), A.end(), 0.0) / A.size();
    X meanB = std::accumulate(B.begin(), B.end(), 0.0) / B.size();

    // Calculating cross-deviation and deviation about x
    X SSxy = multiplyAndSum<T, T>(A, B) - (N * meanA * meanB);
    X SSxx = multiplyAndSum<T, T>(A, A) - (N * meanA * meanA);

    // Calculating regression coefficients
    X slopeB1 = SSxy / SSxx;
    X interceptB0 = meanB - (slopeB1 * meanA);

    // Return vector, insert slope first and then intercept
    std::vector< X > coef;
    coef.push_back(slopeB1);
    coef.push_back(interceptB0);
    return coef;
}

#endif /*__REGRESSION__*/