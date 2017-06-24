//
// Created by 王润基 on 2017/4/19.
//

#include "ImageTexture.h"
#include <fstream>

ImageTexture::ImageTexture(const cv::Mat &image) : image(image) {}

ImageTexture::ImageTexture(std::string filePath) {
    image = cv::imread(filePath);
    if(image.data == NULL)
        throw std::invalid_argument("Failed to read image: " + filePath);
}

Color ImageTexture::operator()(Point const &uv) const {
    int x = int(to01(uv.x / scale.x) * image.size[0]);
    int y = int(to01(uv.y / scale.y) * image.size[1]);
    cv::Vec3b pixel = image.at<cv::Vec3b>(x, y);
//    std::cerr << uv << std::endl;
//    std::cerr << Color(pixel[0], pixel[1], pixel[2]) / 255 << std::endl;
    return Color(pixel[0], pixel[1], pixel[2]) * (k / 255);
}
