#include "filters.h"
#include "masks.h"
#include "color_converter.h"
#include "transformations.h"

namespace project {

void GaussianFilteringY (cv::Mat YCrCb_image, cv::Mat filtered_image) // Nao usada
{
	int rows, cols, region_rows, region_cols;
	const int region_height = 3; // Deve ser um valor impar para ser formada uma mascara com um pixel central
	const int region_width = region_height;
	const int border = (region_height - 1) / 2; // Supoe que region_height = region_width; tamanho da borda
	double color;
	cv::Mat padded_image;
	cv::Mat current_region (region_height, region_width, CV_8UC3);

	// Cria uma borda preta de "border" pixels ao redor da imagem
	cv::copyMakeBorder (YCrCb_image, padded_image, border, border, border, border, cv::BORDER_CONSTANT, 0);

	// Percorre os pixels da imagem que nao sao borda
	for (rows = border; rows < YCrCb_image.rows + border; rows ++)
	{
		for (cols = border; cols < YCrCb_image.cols + border; cols ++)
		{
			// Capturando a regiao vizinha do pixel atual

			for (region_rows = 0; region_rows < region_height; region_rows ++)
			{
				for (region_cols = 0; region_cols < region_width; region_cols ++)
				{
					current_region.at<cv::Vec3b>(region_rows, region_cols) = padded_image.at<cv::Vec3b>(rows + region_rows - border, cols + region_cols - border);
				}
			}

			// Mudando o Y de acordo com a equacao de gauss
			color = GaussianMask (current_region, region_height);
			filtered_image.at<cv::Vec3b>(rows - 1, cols - 1).val[0] = (int) color;
		}
	}
} // GaussianFilteringY

void SaltAndPepperFilteringCb (cv::Mat YCrCb_image, cv::Mat filtered_image)
{
	int rows, cols, region_rows, region_cols;
	const int region_height = 3; // Deve ser um valor impar para existir um pixel central
	const int region_width = region_height;
	const int border = (region_height - 1) / 2; // Supoe que region_height = region_width; tamanho da borda
	double color;
	cv::Mat padded_image;
	cv::Mat current_region (region_height, region_width, CV_8UC3);

	// Cria uma borda que copia os pixels adjacentes de "border" pixels ao redor da imagem
	cv::copyMakeBorder (YCrCb_image, padded_image, border, border, border, border, cv::BORDER_REPLICATE);

	// Percorre os pixels da imagem que nao sao borda
	for (rows = border; rows < YCrCb_image.rows + border; rows ++)
	{
		for (cols = border; cols < YCrCb_image.cols + border; cols ++)
		{
			// Capturando a regiao vizinha do pixel atual

			for (region_rows = 0; region_rows < region_height; region_rows ++)
			{
				for (region_cols = 0; region_cols < region_width; region_cols ++)
				{
					current_region.at<cv::Vec3b>(region_rows, region_cols) = padded_image.at<cv::Vec3b>(rows + region_rows - border, cols + region_cols - border);
				}
			}

			// Mudando o Cb de acordo com a mediana
			color = SaltAndPepperMask (current_region, region_height);
			filtered_image.at<cv::Vec3b>(rows - border, cols - border).val[2] = (int) color;
		}
	}
} // SaltAndPeperFilteringCb

void MultipleImagesAverageY (cv::Mat filtered_image) 
{
	int counter = 1, number_images = 100;
	int rows, cols;\
	int print = 0;
	std::string address = "../images/";
	std::string extension = ".bmp"; // Extensao do arquivo
	std::string number;
	std::string name; // Nome completo do arquivo
	cv::Mat current_image, average_image_Y, average_image;

	// Operacoes primeiro serao feitas em BGR, visto que a transformacao para o espaco Y e linear

	// Para counter = 1; (ja foi verificada a existencia de 1.bmp em main.cc)
	number = std::to_string (counter);
	name = address + number + extension;
	current_image = cv::imread (name);

	if (print)
		std::cout << name << std::endl;

	current_image = current_image / number_images;
	average_image = current_image.clone();

	for (counter = 2; counter <= number_images; counter ++)
	{
		number = std::to_string (counter);
		name = address + number + extension;
		current_image = cv::imread (name);

		if( current_image.empty())
 	 	{
   		std::cout <<" \nError opening image. Result not trustable.\n";
    	std::cout <<" Make sure that the file " << name << " is located in the right directory\n";
    	continue;
  	}
		if (print) 
			std::cout << name << std::endl;

		current_image = current_image / number_images;
		average_image = average_image + current_image; // Soma das matrizes
	}

	average_image_Y = colorcv::CvtColorBGR2GRAY (average_image);

	for (rows = 0; rows < average_image_Y.rows; rows ++)
	{
		for (cols = 0; cols < average_image_Y.cols; cols ++)
		{
			filtered_image.at<cv::Vec3b>(rows, cols).val[0] = average_image_Y.at<uchar>(rows, cols);
		}
	}
}

void NotchFilteringCr (cv::Mat YCrCb_image, cv::Mat filtered_image)
{
	cv::Mat vis_freq_image, save_freq_image;
	int notch_number;
	float normalizer;
	const int number_notches = 12;
	std::vector <cv::Point> notch;
	notch.resize(number_notches);

	notch[0] = cv::Point (288,288);
	notch[2] = cv::Point (432,288);
	notch[4] = cv::Point (144,288);
	notch[6] = cv::Point (576,288);
	notch[8] = cv::Point (288,144);
	notch[10] = cv::Point (432,144);

	for (notch_number = 0; notch_number < number_notches; notch_number += 2)
	{
		// Adquirindo a coluna do notch simetrico

		notch[notch_number + 1].x = (YCrCb_image.cols - 1) - notch[notch_number].x; 

		notch[notch_number + 1].y = (YCrCb_image.rows- 1) - notch[notch_number].y; 
	}

	// Transforma YCrCb_image pra o dominio da frequencia e recentraliza seus elementos
	//  Alem disso, retorna freq_image_cr, a imagem modificada propriamente para visualizacao
	vis_freq_image = FourierTransformCr(YCrCb_image, &normalizer);

	save_freq_image = vis_freq_image * 255;

	cv::imwrite ("fourier.bmp", save_freq_image);
	cv::imshow ("Fourier Cr", vis_freq_image);
	cv::waitKey (0);
	cv::destroyWindow ("Fourier Cr");

	GaussianNotchMask (vis_freq_image, notch);

	save_freq_image = vis_freq_image * 255;

	cv::imwrite ("fourier_filtered.bmp", save_freq_image);
	cv::imshow ("Fourier Filtered Cr", vis_freq_image);
	cv::waitKey (0);
	cv::destroyWindow ("Fourier Filtered Cr");

	FourierInverseTransformCr (vis_freq_image, filtered_image, normalizer);

} // NotchFilteringCr

} // namespace project