#include "transformations.h"

namespace project 
{

cv::Mat FourierTransformCr (cv::Mat YCrCb_image, float* normalizer)
{
	int rows, cols;
	int right_border, bottom_border;
	int best_height, best_width;
	cv::Mat padded_image_cr, mod_freq_image, freq_image_cr;
	cv::Mat char_image;
	cv::Mat YCrCb_image_cr (YCrCb_image.rows, YCrCb_image.cols, CV_32FC1);

	// Copiando a camada Cr da imagem em outra imagem
	for (rows = 0; rows < YCrCb_image.rows; rows ++)
	{
		for(cols = 0; cols < YCrCb_image.cols; cols++)
		{
			YCrCb_image_cr.at<float>(rows, cols) = YCrCb_image.at<cv::Vec3b>(rows, cols).val[1];
		}
	}

	cv::Mat teste (YCrCb_image.rows, YCrCb_image.cols, CV_8UC1);

	for (rows = 0; rows < YCrCb_image.rows; rows ++)
	{
		for(cols = 0; cols < YCrCb_image.cols; cols++)
		{
			teste.at<uchar>(rows, cols) = YCrCb_image.at<cv::Vec3b>(rows, cols).val[1];
		}
	}

	cv::imshow ("teste", teste);
	cv::waitKey (0);
	cv::destroyWindow ("teste");

	// Criar borda de forma a optimizar a transformada

	best_height = cv::getOptimalDFTSize (YCrCb_image.rows);
	best_width = cv::getOptimalDFTSize (YCrCb_image.cols);

	bottom_border = best_height - YCrCb_image_cr.rows;
	right_border = best_width - YCrCb_image_cr.cols;

	copyMakeBorder (YCrCb_image_cr, padded_image_cr, 0, bottom_border, 0, right_border, cv::BORDER_CONSTANT, 0);

	padded_image_cr /= 255;

	cv::dft (padded_image_cr, freq_image_cr, cv::DFT_COMPLEX_OUTPUT);

	// Adequa a matriz gerada no dominio da frequencia e tambem recentraliza a imagem "freq_image_cr"

	mod_freq_image = FreqDomainModifier (freq_image_cr, normalizer);

	return mod_freq_image;
} // FourierTransformCr

// Adequa uma matriz gerada pela dft
cv::Mat FreqDomainModifier (cv::Mat freq_image, float* normalizer)
{
	int rows, cols;
	const float gamma = 0.2;
	double max_value, min_value;
	cv::Mat new_freq_image;
	cv::Mat mod_freq_image (freq_image.rows, freq_image.cols, CV_32FC1);

	// Cria os valores de mod_freq_image com base em freq_image

	for(rows = 0; rows < freq_image.rows; rows ++)
	{
		for(cols = 0; cols < freq_image.cols; cols ++)
		{
			//Atribui o valor da magnitude do numero complexo de freq_image a mod_freq_image

			mod_freq_image.at<float>(rows,cols) = std::sqrt( std::pow(freq_image.at<cv::Vec2f>(rows,cols).val[0], 2) + std::pow(+freq_image.at<cv::Vec2f>(rows,cols).val[1], 2) );

			//Transformacao "gamma" da matriz

			mod_freq_image.at<float>(rows,cols) = (float) std::pow(mod_freq_image.at<float>(rows,cols), gamma);
		} 
	}
  // Centraliza as baixas frequencias da imagem de frequencia original e da modificada

	//freq_image = RecenterFreqDomain (freq_image);

	new_freq_image = RecenterFreqDomain (mod_freq_image);


	// Normalizacao dos valores (0 a 255)
 
		// Pega o maior valor da matriz
	cv::minMaxLoc (new_freq_image, &min_value, &max_value); 
	
	new_freq_image /= (float) max_value;

	*normalizer = (float) max_value;

	return new_freq_image;

}// FreqDomainModifier


// Centraliza as baixar frequencias

cv::Mat RecenterFreqDomain (cv::Mat freq_image)
{
	cv::Mat tmp;
	int new_height = freq_image.rows;
	int new_weight = freq_image.cols;

	// Redimensionar imagem para que tenha dimensoes pares, tirando um pedaco da borda se necessario
	if (freq_image.rows % 2 != 0)
		new_height -= 1;

	if (freq_image.cols % 2 != 0)
		new_weight -= 1;

	cv::Mat new_freq_image (freq_image, cv::Rect (cv::Point(0,0), cv::Point(new_weight, new_height)));

	// Rearranjar os quadrantes
	
	cv::Point center (new_freq_image.cols /2, new_freq_image.rows /2);

	cv::Mat quad_1 (new_freq_image, cv::Rect ( 0, 0, center.x, center.y)); // Superior a esquerda
	cv::Mat quad_2 (new_freq_image, cv::Rect ( center.x, 0, center.x, center.y)); // Superior a direita
	cv::Mat quad_3 (new_freq_image, cv::Rect ( 0, center.y, center.x, center.y)); // Inferior a esquerda
	cv::Mat quad_4 (new_freq_image, cv::Rect ( center.x, center.y, center.x, center.y)); // Inferior a direita

		// Trocar o primeiro quadrante com o quarto

	quad_1.copyTo(tmp);
	quad_4.copyTo(quad_1);
	tmp.copyTo(quad_4);

		// Trocar o segundo quadrante com o terceiro

	quad_2.copyTo(tmp);
	quad_3.copyTo(quad_2);
	tmp.copyTo(quad_3);

	return new_freq_image;
} // RecenterFreqDomain

// Faz a transformada de fourier inversa e recentraliza a imagem
// (filtered_image tem que ter as mesmas dimensoes daquela usada na transformacao)
void FourierInverseTransformCr (cv::Mat freq_image_cr, cv::Mat filtered_image, float normalizer)
{	
	int rows, cols;
	cv::Mat mod_freq_image, padded_image_cr;
	double max_value, min_value;

  // Desfazer as modificacoes feitas para visualizacao
	mod_freq_image = UndoFreqModifier (freq_image_cr, normalizer);

	cv::minMaxLoc (mod_freq_image, &min_value, &max_value);

	std::cout << max_value << " " << min_value << "\n"; 

	cv::imshow ("teste4", mod_freq_image);
	cv::waitKey (0);
	cv::destroyWindow ("teste4");

	// Transformada inversa
	// Restricao de regiao visto que pode ter sido utilizada borda de forma a optimizar a transformada

	cv::dft (mod_freq_image, padded_image_cr, cv::DFT_INVERSE + cv::DFT_SCALE/*, filtered_image.rows*/);

	// Regiao de interesse

	cv::Mat new_image_cr (padded_image_cr, cv::Rect(0, 0, filtered_image.rows, filtered_image.cols));

	// Compensa a divisao feita antes da transformacao

	//new_image_cr *= 255;

	//new_image_cr.convertTo(new_image_cr, CV_8U);

	cv::imshow ("teste2", new_image_cr);
	cv::waitKey (0);
	cv::destroyWindow ("teste2");

	for(rows = 0; rows < filtered_image.rows; rows++)
	{
		for(cols = 0; cols < filtered_image.cols; cols++)
		{
			filtered_image.at<cv::Vec3b>(rows, cols).val[1] = new_image_cr.at<uchar>(rows, cols);
		}
	}

} // FourierInverseTransform

// Freq_image de 1 canal
cv::Mat UndoFreqModifier (cv::Mat freq_image, float normalizer)
{
	int rows, cols;
	const float gamma = 5;
	double max_value, min_value;
	cv::Mat new_freq_image;
	cv::Mat mod_freq_image (freq_image.rows, freq_image.cols, CV_32FC1);

	// Desfaz a normalizacao feita
	freq_image *= normalizer;
	
	// Cria os valores de mod_freq_image com base em freq_image

	for(rows = 0; rows < freq_image.rows; rows ++)
	{
		for(cols = 0; cols < freq_image.cols; cols ++)
		{
			//Transformacao "gamma" da matriz

			mod_freq_image.at<float>(rows,cols) = (float) std::pow(freq_image.at<float>(rows,cols), gamma);
		} 
	}

	cv::minMaxLoc (mod_freq_image, &min_value, &max_value);

	std::cout << max_value << " " << min_value << "\n"; 

	cv::imshow ("teste7", mod_freq_image);
	cv::waitKey (0);
	cv::destroyWindow ("teste7");

	new_freq_image = RecenterFreqDomain (mod_freq_image);

	return new_freq_image;
} // UndoFreqModifier

}