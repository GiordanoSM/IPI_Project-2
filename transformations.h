#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <iostream>

namespace project 
{

cv::Mat FourierTransformCr (cv::Mat YCrCb_image, float* normalizer);

cv::Mat FreqDomainModifier (cv::Mat freq_image, float* normalizer);

cv::Mat RecenterFreqDomain (cv::Mat freq_image);

void FourierInverseTransformCr (cv::Mat freq_image_cr, cv::Mat filtered_image, float normalizer);

cv::Mat UndoFreqModifier (cv::Mat freq_image, float normalizer);
}

#endif