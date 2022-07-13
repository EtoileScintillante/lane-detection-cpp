#include "regression.h"

int multiplyAndSum(std::vector<int> a, std::vector<int> b)
{
    int sum;
    std::vector<int> temp;
    for (int i = 0; i < a.size(); i++)
    {
        temp.push_back(a[i]*b[i]);
    }
    sum = std::accumulate(temp.begin(), temp.end(), 0.0);

    return sum;
}

std::vector<double> estimateCoefficients(std::vector<int> X, std::vector<int> Y)
{
    // Sample size
    int N = X.size();

    // Calculate mean of X and Y
    double meanX = std::accumulate(X.begin(), X.end(), 0.0) / X.size();
    double meanY = std::accumulate(Y.begin(), Y.end(), 0.0) / Y.size();

    // Calculating cross-deviation and deviation about x
    double SSxy = multiplyAndSum(X, Y) - (N * meanX * meanY);
    double SSxx = multiplyAndSum(X, X) - (N * meanX * meanX);

    // Calculating regression coefficients
    double slopeB1 = SSxy / SSxx;
    double interceptB0 = meanY - (slopeB1 * meanX);

    // Return vector, insert slope first and then intercept
    std::vector< double > coef;
    coef.push_back(slopeB1);
    coef.push_back(interceptB0);
    return coef;
}