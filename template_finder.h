#ifndef TEMPLATE_FINDER_H
#define TEMPLATE_FINDER_H

#include <opencv2/opencv.hpp>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <string>

namespace project {

void GetTemplates (cv::Mat base_image, cv::Point start, cv::Point end);

cv::Mat RotateImage (cv::Mat original_image, cv::Mat rotated_image, double angle, cv::Point rotation_center);

} // namespace project

#endif