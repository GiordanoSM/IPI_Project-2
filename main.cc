#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

#include "filters.h"
#include "masks.h"
#include "template_finder.h"
#include "color_converter.h"

int main ()
{
	cv::Point start(410,494), end(496,611); // Regiao do template do aviao
	cv::Mat airport, ycc_airport, filtered_image_1, filtered_image_2, filtered_image_3, final_image;

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

	ycc_airport = colorcv::CvtColorBGR2YCrCb (airport);

	cv::imshow ("YCrCb Image", ycc_airport);
	cv::waitKey (0);
	cv::destroyWindow ("YCrCb Image");

	filtered_image_1 = ycc_airport.clone();

	project::MultipleImagesAverageY (filtered_image_1);

	//project::GaussianFilteringY (ycc_airport, filtered_image_1); // Nao sera utilizada

	// Primeira filtragem (Y)

	final_image = colorcv::CvtColorYCrCb2BGR (filtered_image_1);

	cv::imshow ("After First Filter", final_image);
	cv::waitKey (0);
	cv::destroyWindow ("After First Filter");

	// Segunda filtragem (Cb)

	filtered_image_2 = filtered_image_1.clone();

	project::SaltAndPepperFilteringCb (filtered_image_1, filtered_image_2);
	
	final_image = colorcv::CvtColorYCrCb2BGR (filtered_image_2);

	cv::imshow ("After Second Filter", final_image);
	cv::waitKey (0);
	cv::destroyWindow ("After Second Filter");

  // Terceira filtragem (Cr)

	filtered_image_3 = filtered_image_2.clone();

	//project::NotchFilteringCr (filtered_image_2, filtered_image_3);

	final_image = colorcv::CvtColorYCrCb2BGR (filtered_image_3);

	cv::imshow ("After Third Filter", final_image);
	cv::waitKey (0);
	cv::destroyWindow ("After Third Filter");

	cv::imwrite ("denoised_image.bmp", final_image);
	cv::destroyAllWindows();

	project::GetTemplates (filtered_image_2, start, end);

	project::TemplateCorrelation (filtered_image_2);
}
