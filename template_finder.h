#ifndef TEMPLATE_FINDER_H
#define TEMPLATE_FINDER_H

#include <opencv2/opencv.hpp>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

namespace project {

void GetTemplates (cv::Mat base_image, cv::Point start, cv::Point end);

cv::Mat RotateImage (cv::Mat original_image, cv::Mat rotated_image, double angle, cv::Point rotation_center);

void TemplateCorrelation (cv::Mat image);

cv::Vec3d NormCrossCorrelation3C (cv::Mat current_region, cv::Vec3d region_average, std::vector <cv::Vec3d> &template_average, int template_number);

} // namespace project

#endif