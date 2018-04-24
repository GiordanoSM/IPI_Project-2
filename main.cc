#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

#include "filters.h"
#include "masks.h"

int main ()
{
	cv::Mat airport, ycc_airport, filtered_image_1, filtered_image_2, final_image;

	airport = cv::imread ("1.bmp");

	    // Check if image is loaded fine
  if( airport.empty())
  {
    printf(" Error opening image\n");
    printf(" Make sure that the file 1.bmp is in the same directory of your executable\n");
    return -1;
  }

	cv::cvtColor (airport, ycc_airport, cv::COLOR_BGR2YCrCb);

	filtered_image_1 = ycc_airport.clone();

	project::GaussianFilteringY (ycc_airport, filtered_image_1);

	filtered_image_2 = filtered_image_1.clone();

	cv::cvtColor (filtered_image_1, final_image, cv::COLOR_YCrCb2BGR);

	cv::imshow ("final", final_image);
	cv::waitKey (0);

	project::SaltAndPepperFilteringCb (filtered_image_1, filtered_image_2);
	
	cv::cvtColor (filtered_image_2, final_image, cv::COLOR_YCrCb2BGR);


	cv::imshow ("airport", airport);
	cv::waitKey (0);
	cv::imshow ("ycc", ycc_airport);
	cv::waitKey (0);
	cv::imshow ("final_2", final_image);
	cv::waitKey (0);
}
