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

} // namespace project