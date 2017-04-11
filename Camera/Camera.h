//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_CAMERA_H
#define INC_2RAYTRACE_CAMERA_H


#include "../Geometry/Ray.h"

class Camera {
public:
    virtual void render() const = 0;
};


#endif //INC_2RAYTRACE_CAMERA_H
