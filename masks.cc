#include "masks.h"
#include "sorting.h"

namespace project {

// Parametro: Vizinhanca "dimension" x "dimension" do ponto atual 
double GaussianMask (cv::Mat current_region, const int dimension) 
{
	int x_relative, y_relative; // Posicoes relativas ao pixel central
	int cols, rows, total_weight = 0; 
	double color = 0;
	double std_deviation = 1; // Ira variar dependendo da dimensao do filtro e do resultado buscado
	const int neighborhood = (dimension - 1)/2; // A largura dos vizinhos para cada lado do ponto central

	cv::Mat gaussian_mask (dimension, dimension, CV_8UC1); // Filtro dimension x dimension

	// Criando o filtro gaussiano

	for (y_relative = neighborhood; y_relative >= - neighborhood; y_relative --)
	{
		for (x_relative = - neighborhood;x_relative <= neighborhood; x_relative ++)
		{
			// Calculando os pesos (posicao relativa + neighborhood = posicao na matriz)

			gaussian_mask.at<uchar>(y_relative + neighborhood, x_relative + neighborhood) = (int) 50 * std::exp(-1*(std::pow(x_relative,2) + std::pow(y_relative,2)) / (2*std::pow(std_deviation, 2)));
			total_weight += gaussian_mask.at<uchar>(y_relative + neighborhood, x_relative + neighborhood);

		}
	}

	// Aplicando a mascara

	for (rows = 0; rows < dimension; rows++)
	{
		for (cols = 0; cols < dimension; cols++)
		{
			color += gaussian_mask.at<uchar>(rows, cols) * current_region.at<cv::Vec3b>(rows, cols).val[0]; 
		}
	}

	color /= total_weight;

	return color;
} // GaussianMask

// Parametro: Vizinhanca "dimension" x "dimension" do ponto atual 
double SaltAndPepperMask (cv::Mat current_region, const int dimension) 
{
	int cols, rows;
	const int vector_size = dimension*dimension;
	int counter = 0, median = (vector_size - 1)/2;
	double color = 0;
	int sorted_values[vector_size];

	// Extraindo os valores

	for (rows = 0; rows < dimension; rows++)
	{
		for (cols = 0; cols < dimension; cols++, counter++)
		{
			sorted_values [counter] = current_region.at<cv::Vec3b>(rows, cols).val[2]; 
		}
	}

	sort::InsertionSort(sorted_values, vector_size); 

	color = sorted_values [median];

	return color;
} // SaltAndPeperMask

//Imagem no dominio da frequencia a ser filtrada e os pontos a serem retirados
void GaussianNotchMask (cv::Mat freq_image, std::vector <cv::Point> notch)
{
	int rows, cols;
	unsigned int notch_number;
	double mask_value;
	const unsigned int radius = 10; // Distancia escolhida como a distancia de filtragem a partir do notch
	std::vector <double> distance_notch;
	distance_notch.resize(notch.size());

	// Multiplicador que determina ate em quantas vezes a distancia de filtragem o filtro continua relevante 
	const float distance_multiplier = 2.5;


	for (rows = 0; rows < freq_image.rows; rows++)
	{
		for (cols = 0; cols < freq_image.cols; cols++)
		{
			for (notch_number = 0, mask_value = 1; notch_number < notch.size(); notch_number++)
			{
				distance_notch[notch_number] = std::sqrt (std::pow( (float)(cols - notch[notch_number].x) ,2) + std::pow( (float) (rows - notch[notch_number].y) ,2));

				// Condicao para poupar calculos quando a diferenca do resultado do filtro para "1" e desconsideravel
				if ( distance_notch[notch_number] < distance_multiplier * radius )
				{
					// Equacao da filtragem passa-altas gaussiana.
					mask_value *= 1 - std::exp( (-1* std::pow (distance_notch[notch_number],2)) / (2* std::pow(radius,2)) ); 
				}

			} // notch_number

			freq_image.at<float>(rows, cols) = freq_image.at<float>(rows,cols) * mask_value;
		} // cols
	} // rows
} // GaussianNotchMask

} // namespace project