//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_IRAYCASTABLE_H
#define INC_2RAYTRACE_IRAYCASTABLE_H

#include "Vector3.h"
#include "Ray.h"

class IRayCastable {
public:
    virtual bool tryGetIntersectionPoint(Ray const &ray, float& t) const = 0;
};

#endif //INC_2RAYTRACE_IRAYCASTABLE_H
