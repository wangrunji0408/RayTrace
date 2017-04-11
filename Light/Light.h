//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_LIGHT_H
#define INC_2RAYTRACE_LIGHT_H

#include "../Geometry/Geometry.h"

typedef Vector3f Color;

class Light {
public:
    Ray ray;
    Color color;

    Light(const Ray &ray, const Color &color) : ray(ray), color(color) {}
};


#endif //INC_2RAYTRACE_LIGHT_H
