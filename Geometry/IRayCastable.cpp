//
// Created by 王润基 on 2017/4/8.
//

#include "IRayCastable.h"

bool IRayCastable::tryGetIntersectionPoint(Ray const &ray, Vector3f &point) const {
    float t;
    if(!tryGetIntersectionPoint(ray, t))
        return false;
    point = ray.getEndPoint(t);
    return true;
}