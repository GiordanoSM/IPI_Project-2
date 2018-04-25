#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <opencv2/core/mat.hpp>

namespace project {

// Aplica a filtragem gaussiana na camada Y da primeira imagem passada como parametro e coloca o resultado na segunda.

void GaussianFilteringY (cv::Mat YCrCb_image, cv::Mat filtered_image);

// Aplica a filtragem "salt_and_pepper" na camada Cb da primeira imagem passada como parametro e coloca o resultado na segunda.

void SaltAndPepperFilteringCb (cv::Mat YCrCb_image, cv::Mat filtered_image);

// Aplica a media dos valores de cada pixel na camada Y de 100 imagens iguais com ruidos diferentes

void MultipleImagesAverageY (cv::Mat filtered_image);

} // namespace project


#endif