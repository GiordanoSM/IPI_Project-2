#ifndef MASKS_H
#define MASKS_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <iostream>
#include <cmath>

namespace project {

double GaussianMask (cv::Mat current_region, const int dimension);

double SaltAndPepperMask (cv::Mat current_region, const int dimension);

} // namespace project

#endif