//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_LIGHT_H
#define INC_2RAYTRACE_LIGHT_H

#include <ostream>
#include "../Geometry/Geometry.h"

typedef Vector3f Color;

class Light {
public:
    Ray ray;
    Color color;
public:
    Light(const Ray &ray, const Color &color) : ray(ray), color(color) {}
    Vector3f getUnitDir () const;

    friend std::ostream &operator<<(std::ostream &os, const Light &light);
};


#endif //INC_2RAYTRACE_LIGHT_H
