//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_IMAGETEXTURE_H
#define INC_2RAYTRACE_IMAGETEXTURE_H


#include <opencv2/opencv.hpp>
#include "Texture.h"

class ImageTexture: public Texture {
    cv::Mat image;
public:
    ImageTexture(const cv::Mat &image);
    ImageTexture(std::string filePath);
    Color operator()(Point const &uv) const override;
};


#endif //INC_2RAYTRACE_IMAGETEXTURE_H
