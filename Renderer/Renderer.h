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

    int round = 1;
    int minSampleTimes = 5;     // 每轮最大最小采样次数
    int maxSampleTimes = 10;
    float s2 = 0;
protected:
    int totalTime = 0;
    int h, w;
    int renderTimes = 0;
    cv::Mat3f mat;
public:
    int getRenderTimes() const;

protected:
    shared_ptr<World> world;
    shared_ptr<Camera> camera;
protected:
    static void recolor (cv::Mat3f &mat);
    virtual Color renderPixel(int x, int y) const;
    virtual Color renderRay (Ray const& ray) const = 0;
public:
    Renderer(shared_ptr<World> world, shared_ptr<Camera> camera);

    void clear();
    bool render (); // 继续渲染，返回是否完成
    cv::Mat getResult() const;
    shared_ptr<Camera> getCamera() const;
};


#endif //INC_2RAYTRACE_RENDERER_H
