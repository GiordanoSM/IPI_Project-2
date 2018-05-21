#include "template_finder.h"

namespace project {

void GetTemplates (cv::Mat base_image, cv::Point start, cv::Point end)
{
	double angle;
	int counter = 1;
	std::string address = "../templates/template_";
	std::string extension = ".bmp"; // Extensao do arquivo
	std::string number;
	std::string name;
	cv::Mat obj_template, rotated_image;
	cv::Point template_center ((start.x + end.x)/2, (start.y + end.y)/2);

	// Rotaciona a imagem antes de capturar o template, evitando regioes pretas
	for (angle = -45; angle <=45; angle += 9, counter ++) 
	{
		rotated_image = RotateImage (base_image, rotated_image, angle, template_center);

		cv::Mat region (rotated_image, cv::Rect(start, end));

		obj_template = region.clone(); // Criado um template da regiao em questao

		cv::imshow("Plane template", obj_template);
		cv::waitKey(0);

		number = std::to_string (counter);
		name = address + number + extension;

		cv::imwrite(name, obj_template);
	} 
	cv::destroyAllWindows();
}

cv::Mat RotateImage (cv::Mat original_image, cv::Mat rotated_image, double angle, cv::Point rotation_center)
{
	cv::Mat rotation;
	cv::Rect border;
	cv::Point image_center ( (original_image.cols - 1) /2 , (original_image.rows - 1) /2 );

	// Matriz de transformacao de rotacao
	rotation = cv::getRotationMatrix2D (rotation_center, angle, 1);

	// Cria a imagem rotacionada
	cv::warpAffine (original_image, rotated_image, rotation, original_image.size());

	return rotated_image;
}

void TemplateCorrelation (cv::Mat image)
{
	const int number_templates = 11;
	int rows, cols, region_cols, region_rows, template_pixels;		
	int border_height, border_width, template_number;
	std::string answer = "Os avioes se encontram, aproximadamente, nos seguintes pixels da imagem:";
	cv::Mat current_template, padded_image;
	cv::Vec3d region_average;
	std::vector <cv::Vec3d> template_average;
	cv::Vec3d result;

	current_template = cv::imread ("../templates/template_1.bmp");

	// Visto que os templates tem o mesmo tamanho, cria uma imagem com a borda necessaria para realizar a correlacao
	border_height = (int) (current_template.rows - 1) /2;
	border_width = (int) (current_template.cols - 1) /2;

	template_pixels = current_template.rows * current_template.cols;

	cv::copyMakeBorder (image, padded_image, border_height, border_height, border_width, border_width, cv::BORDER_CONSTANT, 0);
		

	// Visto que os templates tem o mesmo tamanho, cria a matriz que armazenara a regiao ao redor de um pixel	
	cv::Mat current_region (current_template.rows, current_template.cols, CV_8UC3);

	// Percorre os pixels da imagem que nao sao borda
	//for (rows = border_height; rows < image.rows + border_height; rows ++)
	//{
	//	for (cols = border_width; cols < image.cols + border_width; cols ++)
	//	{
			// Capturando a regiao vizinha do pixel atual e faz a media dos valores para cada canal Y Cr Cb
			rows = 611;
			cols = 495;
			region_average = 0;
			for (region_rows = 0; region_rows < current_template.rows; region_rows ++)
			{
				for (region_cols = 0; region_cols < current_template.cols; region_cols ++)
				{
					current_region.at<cv::Vec3b>(region_rows, region_cols) = padded_image.at<cv::Vec3b>(rows + region_rows - border_height, cols + region_cols - border_width);
					region_average += current_region.at<cv::Vec3d>(region_rows, region_cols) / template_pixels;
				}
			}

			for (template_number = 0; template_number < number_templates; template_number ++) // Comparando com todos os templates
			{
				// Calcula o valor da "Normalized Cross-Correlation" do template em questao no pixel atual
				result = NormCrossCorrelation3C (current_region, region_average, template_average, template_number);

		  	// Guarda a localizacao de quando o pixel centraliza um aviao em uma string
				if (result.val[0] + result.val[1] + result.val[2] > 1)
				{
					answer += " ( " + std::to_string(rows - border_height) + ", " + std::to_string(cols - border_width) + ")"; 
				}
			} // template_number

	//	} // cols
	//} // rows

	std::cout << answer << ".\n";

} // TemplateCorrelation

cv::Vec3d NormCrossCorrelation3C (cv::Mat current_region, cv::Vec3d region_average, std::vector <cv::Vec3d> &template_average, int template_number)
{
	int rows, cols, channel;
	cv::Vec3d result;
	cv::Vec3d correlation, denominator;
	cv::Vec3d region_average_diference_sq = 0;

	int template_pixels;
	std::string address = "../templates/template_";
	std::string extension = ".bmp"; // Extensao do arquivo
	std::string number;
	std::string name;

	cv::Vec3d template_average_diference_sq = 0;

	cv::Mat current_template;

	number = std::to_string (template_number + 1); // De 1 a 11
	name = address + number + extension;
	current_template = cv::imread (name);

	template_pixels = current_template.rows * current_template.cols;

	// Calcula o valor medio do template em cada um dos canais Y Cr Cb se primeira vez calculado

	if (template_average.size() < 11)
	{	
		template_average.push_back(0);
		for (rows = 0; rows < current_template.rows; rows++)
		{
			for (cols = 0; cols < current_template.cols; cols++)
			{
				template_average[template_number] += current_template.at<cv::Vec3d>(rows, cols)/ template_pixels;
			}
		}
	}	

	for (rows = 0; rows < current_region.rows; rows ++)
	{
		for (cols = 0; cols < current_region.cols; cols ++)
		{
			// Correlacao
			correlation += (current_region.at<cv::Vec3d>(rows, cols) - region_average) * (current_template.at<cv::Vec3d>(rows, cols) - template_average[template_number]);

			// Variancias
			region_average_diference_sq += (current_region.at<cv::Vec3d>(rows, cols) - region_average).mul((current_region.at<cv::Vec3d>(rows, cols) - region_average));

			template_average_diference_sq += (current_template.at<cv::Vec3d>(rows, cols) - template_average[template_number]).mul(current_template.at<cv::Vec3d>(rows, cols) - template_average[template_number]);
		} 
	}	

	for (channel = 0; channel < 3; channel ++)
	{
		// Multiplicacao dos desvios padroes
		denominator.val[channel] = std::sqrt ( template_average_diference_sq.val[channel] * region_average_diference_sq.val[channel]);

		result.val[channel] = correlation.val[channel] / denominator.val[channel];
	}

	return result;
} // NormCrossCorrelation3C

} // namespace project