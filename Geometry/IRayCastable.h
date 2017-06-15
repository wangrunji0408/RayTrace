//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_IRAYCASTABLE_H
#define INC_2RAYTRACE_IRAYCASTABLE_H

#include "Vector3.h"
#include "Ray.h"

class IRayCastable {
public:
    // 以下二者选一实现
    // 若为参数曲线/曲面，则实现第二个，param为交点参数，用于参数曲面求交
    // 若实现第一个，则param为交点
    virtual bool tryGetIntersectionPoint(Ray const &ray, float& t) const;
    virtual bool tryGetIntersectionPoint(Ray const &ray, float& t, Vector3f& param) const;
};

#endif //INC_2RAYTRACE_IRAYCASTABLE_H
