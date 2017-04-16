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
    Vector3f begin, end;
    Color color;
public:
    Light(const Vector3f &begin, const Vector3f &end, const Color &color);
    Vector3f getUnitDir () const;
    Ray getRay () const;
    float len () const;
    friend std::ostream &operator<<(std::ostream &os, const Light &light);
};


#endif //INC_2RAYTRACE_LIGHT_H
