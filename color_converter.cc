#include "color_converter.h"

namespace colorcv {

cv::Mat CvtColorBGR2GRAY (cv::Mat original_image)
{
	int rows, cols, red, blue, green;
	double gray;
	cv::Mat gray_image;

	if (original_image.depth() == CV_8U)
	{
		gray_image = cv::Mat (original_image.rows, original_image.cols, CV_8UC1);
	}

	else if (original_image.depth() == CV_16U)
	{
		gray_image = cv::Mat (original_image.rows, original_image.cols, CV_16UC1);
	}

	else
	{ 
		std::cout << "This function doesn't realize the convertion of your matrix data type.\n";
		return original_image;
	}

	for (rows = 0; rows < original_image.rows; rows ++)
	{
		for (cols = 0; cols < original_image.cols; cols ++)
		{
			blue = original_image.at<cv::Vec3b>(rows, cols).val[0];
			red = original_image.at<cv::Vec3b>(rows, cols).val[2];
			green = original_image.at<cv::Vec3b>(rows, cols).val[1];
			gray = (0.114 * blue + 0.587 * green + 0.299 * red);
			gray_image.at<uchar>(rows, cols) = (uchar) gray;			
		}
	}
	return gray_image;
} // BGR2GRAY

cv::Mat CvtColorBGR2YCrCb (cv::Mat original_image)
{
	int rows, cols, delta, blue, red, green; // Delta ajusta os valores
	double Y, Cr, Cb;
	cv::Mat YCrCb_image;

	if (original_image.depth() == CV_8U)
	{
		delta = 128;
		YCrCb_image = cv::Mat (original_image.rows, original_image.cols, CV_8UC3);
	}

	else if (original_image.depth() == CV_16U)
	{
		delta = 32768;
		YCrCb_image = cv::Mat (original_image.rows, original_image.cols, CV_16UC3);
	}

	else
	{ 
		std::cout << "This function doesn't realize the convertion of your matrix data type.\n";
		return original_image;
	}

	for (rows = 0; rows < original_image.rows; rows ++)
	{
		for (cols = 0; cols < original_image.cols; cols ++)
		{
			blue = original_image.at<cv::Vec3b>(rows, cols).val[0];
			red = original_image.at<cv::Vec3b>(rows, cols).val[2];
			green = original_image.at<cv::Vec3b>(rows, cols).val[1];

			Y = (0.114 * blue + 0.587 * green + 0.299 * red);
			YCrCb_image.at<cv::Vec3b>(rows, cols).val[0] = (uchar) Y;	

			Cr = ((red - Y) * 0.713 + delta);
			YCrCb_image.at<cv::Vec3b>(rows, cols).val[1] = (uchar) Cr;	

			Cb = ((blue - Y) * 0.564 + delta);
			YCrCb_image.at<cv::Vec3b>(rows, cols).val[2] = (uchar) Cb;	
		}
	}

	return YCrCb_image;
} // BRG2YCrCb

cv::Mat CvtColorYCrCb2BGR (cv::Mat original_image)
{
	int rows, cols, delta, Y, Cr, Cb; // Delta ajusta os valores
	double red, green, blue;
	cv::Mat BGR_image;

	if (original_image.depth() == CV_8U)
	{
		delta = 128;
		BGR_image = cv::Mat (original_image.rows, original_image.cols, CV_8UC3);
	}

	else if (original_image.depth() == CV_16U)
	{
		delta = 32768;
		BGR_image = cv::Mat (original_image.rows, original_image.cols, CV_16UC3);
	}

	else
	{ 
		std::cout << "This function doesn't realize the convertion of your matrix data type.\n";
		return original_image;
	}

	for (rows = 0; rows < original_image.rows; rows ++)
	{
		for (cols = 0; cols < original_image.cols; cols ++)
		{
			Y = original_image.at<cv::Vec3b>(rows, cols).val[0];
			Cb = original_image.at<cv::Vec3b>(rows, cols).val[2];
			Cr = original_image.at<cv::Vec3b>(rows, cols).val[1];

			red = (Y + 1.403 * (Cr - delta));
			if (red < 0)
				red = 0;
			BGR_image.at<cv::Vec3b>(rows, cols).val[2] = (uchar) red;	

			green = (Y - 0.714 * (Cr - delta) - 0.344 * (Cb - delta));
			if (green < 0)
				green = 0;
			BGR_image.at<cv::Vec3b>(rows, cols).val[1] = (uchar) green;	

			blue = (Y + 1.773 * (Cb - delta));
			if (blue < 0)
				blue = 0;
			BGR_image.at<cv::Vec3b>(rows, cols).val[0] = (uchar) blue;	
		}
	}

	return BGR_image;
} // YCrCb2BGR
	
} // namespace colorcv