//
// Created by 王润基 on 2017/4/15.
//

#include "Renderer.h"

Renderer::Renderer(World *world, Camera *camera) : world(world), camera(camera) {}

cv::Mat Renderer::render() const {
    int h = camera->getHeight(), w = camera->getWidth();
    cv::Mat3f mat = cv::Mat(h, w, CV_8UC3);
    for(int i=0; i<h; ++i)
        for(int j=0; j<w; ++j)
            mat(i, j) = renderPixel(i, j);
    return mat;
}
