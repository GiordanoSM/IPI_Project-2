#include "filters.h"
#include "masks.h"

namespace project {

void GaussianFilteringY (cv::Mat YCrCb_image, cv::Mat filtered_image) 
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

} // namespace project