//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_IRAYCASTABLE_H
#define INC_2RAYTRACE_IRAYCASTABLE_H

#include "Vector3.h"
#include "Ray.h"
#include "IntersectInfo.h"

class IRayCastable {
public:
    virtual bool tryGetIntersectionPoint(Ray const &ray, float& t) const;
    virtual bool tryGetIntersectionPoint(Ray const &ray, float& t, Vector3f& param) const;
    virtual void intersect (IntersectInfo &info) const;
};

#endif //INC_2RAYTRACE_IRAYCASTABLE_H
