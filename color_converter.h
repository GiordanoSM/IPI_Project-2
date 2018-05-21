#ifndef COLOR_CONVERTER_H
#define COLOR_CONVERTER_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>

namespace colorcv {
	
cv::Mat CvtColorBGR2GRAY (cv::Mat original_image);	

cv::Mat CvtColorBGR2YCrCb (cv::Mat original_image);

cv::Mat CvtColorYCrCb2BGR (cv::Mat original_image);

}

#endif