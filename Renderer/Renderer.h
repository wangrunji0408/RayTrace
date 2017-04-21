//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_RENDERER_H
#define INC_2RAYTRACE_RENDERER_H

#include <opencv2/opencv.hpp>
#include "../World/World.h"
#include "../Camera/Camera.h"

class Renderer {
public:
    string name = "renderer";
    bool enableRecolor = false;
    bool enableParallel = true;
    bool super = false;
protected:
    shared_ptr<World> world;
    shared_ptr<Camera> camera;
protected:
    static void recolor (cv::Mat3f &mat);
public:
    Renderer(shared_ptr<World> world, shared_ptr<Camera> camera);
    virtual Color renderPixel(int x, int y) const = 0;

    virtual cv::Mat render() const;
    cv::Mat render8U3C() const;
    shared_ptr<Camera> getCamera() const;
};


#endif //INC_2RAYTRACE_RENDERER_H
