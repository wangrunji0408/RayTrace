//
// Created by 王润基 on 2017/4/15.
//

#include "Renderer.h"

Renderer::Renderer(World *world, Camera *camera, bool useRecolor) :
        world(world), camera(camera), useRecolor(useRecolor) {}

cv::Mat3f Renderer::render() const {
    int h = camera->getHeight(), w = camera->getWidth();
    cv::Mat3f mat = cv::Mat(h, w, CV_32FC3);
    mat.forEach([this](cv::Vec3f & p, const int id[])
                {
                    p = renderPixel(id[0], id[1]);
                });
    if(useRecolor)
        recolor(mat);
    return mat;
}

void Renderer::recolor(cv::Mat3f &mat) {
//    cv::Vec3f maxPixel(0, 0, 0);
    float maxValue = 0;
    mat.forEach([&](cv::Vec3f const& p, const int * position)
                {
                    maxValue = std::max(maxValue, p[0]);
                    maxValue = std::max(maxValue, p[1]);
                    maxValue = std::max(maxValue, p[2]);
//                    maxPixel[0] = std::max(maxPixel[0], p[0]);
//                    maxPixel[1] = std::max(maxPixel[1], p[1]);
//                    maxPixel[2] = std::max(maxPixel[2], p[2]);
                });
    mat.forEach([&](cv::Vec3f & p, const int * position)
                {
                    p[0] /= maxValue;
                    p[1] /= maxValue;
                    p[2] /= maxValue;
                });
}
