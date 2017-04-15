//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_RENDERER_H
#define INC_2RAYTRACE_RENDERER_H

#include <opencv2/opencv.hpp>
#include "../World/World.h"
#include "../Camera/Camera.h"

class Renderer {
protected:
    World* world;
    Camera* camera;
public:
    Renderer(World *world, Camera *camera);
    virtual cv::Vec3f renderPixel (int x, int y) const = 0;
    cv::Mat render() const;
};


#endif //INC_2RAYTRACE_RENDERER_H
