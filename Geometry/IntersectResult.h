//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_INTERSECTRESULT_H
#define INC_2RAYTRACE_INTERSECTRESULT_H


#include "Vector3.h"
#include "../Shapes/Shape.h"

struct IntersectResult {
    const bool success;
    const Vector3f intersectPoint;
    const float t;
    const Shape* object;

    IntersectResult(const bool success, const Vector3f &intersectPoint, const float t, const Shape *object) : success(
            success), intersectPoint(intersectPoint), t(t), object(object) {}
};


#endif //INC_2RAYTRACE_INTERSECTRESULT_H
