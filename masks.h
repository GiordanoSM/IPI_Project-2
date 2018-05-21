#ifndef MASKS_H
#define MASKS_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <vector>

namespace project {

double GaussianMask (cv::Mat current_region, const int dimension);

double SaltAndPepperMask (cv::Mat current_region, const int dimension);

void GaussianNotchMask (cv::Mat freq_image, std::vector <cv::Point> notch);

} // namespace project

#endif