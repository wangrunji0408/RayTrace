//
// Created by 王润基 on 2017/4/15.
//

#include "Renderer.h"

Renderer::Renderer(World *world, Camera *camera) :
        world(world), camera(camera) {}

cv::Mat Renderer::render() const {
    time_t t0 = time(0);
    if(super)
        camera->setResolution(camera->getWidth() * 2, camera->getHeight() * 2);
    int h = camera->getHeight(), w = camera->getWidth();
    cv::Mat3f mat = cv::Mat(h, w, CV_32FC3);
    if(enableParallel)
        mat.forEach([this](cv::Vec3f & p, const int id[])
                {
                    p = renderPixel(id[0], id[1]);
                });
    else
        for(int i=0; i<h; ++i)
            for(int j=0; j<w; ++j)
                mat[i][j] = renderPixel(i, j);
    if(enableRecolor)
        recolor(mat);
    auto t= time(0) - t0;
    std::cerr << "Render End. Time = " << t << "s" << std::endl;
    if(super)
    {
        camera->setResolution(w/2, h/2);
        cv::Mat3f mat1;
        cv::resize(mat, mat1, cv::Size(w/2, h/2));
        return mat1;
    }
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

cv::Mat Renderer::render8U3C() const {
    cv::Mat mat = render() * 255;
    cv::Mat mat1;
    mat.convertTo(mat1, CV_8UC3);
    return mat1;
}

Camera *Renderer::getCamera() const {
    return camera;
}
