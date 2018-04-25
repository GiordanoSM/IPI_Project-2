#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

#include "filters.h"
#include "masks.h"

int main ()
{
	cv::Point start, end;
	cv::Mat airport, ycc_airport, filtered_image_1, filtered_image_2, final_image;

	airport = cv::imread ("../images/1.bmp");

	// Check if image is loaded fine
  if( airport.empty())
  {
    std::cout << " Error opening image\n";
    std::cout << " Make sure that the file (../images/1.bmp) is located in the right directory\n";
    return -1;
  }

  cv::imshow ("Original Image", airport);
	cv::waitKey (0);

	cv::cvtColor (airport, ycc_airport, cv::COLOR_BGR2YCrCb);

	cv::imshow ("YCrCb Image", ycc_airport);
	cv::waitKey (0);

	filtered_image_1 = ycc_airport.clone();

	project::MultipleImagesAverageY (filtered_image_1);

	//project::GaussianFilteringY (ycc_airport, filtered_image_1);

	filtered_image_2 = filtered_image_1.clone();

	cv::cvtColor (filtered_image_1, final_image, cv::COLOR_YCrCb2BGR);

	cv::imshow ("After First Filter", final_image);
	cv::waitKey (0);

	project::SaltAndPepperFilteringCb (filtered_image_1, filtered_image_2);
	
	cv::cvtColor (filtered_image_2, final_image, cv::COLOR_YCrCb2BGR);

	cv::imshow ("After Second Filter", final_image);
	cv::waitKey (0);
	cv::imwrite ("denoised_image.bmp", final_image);


}
