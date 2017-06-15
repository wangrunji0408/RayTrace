//
// Created by 王润基 on 2017/4/8.
//

#include "IRayCastable.h"

bool IRayCastable::tryGetIntersectionPoint(const Ray &ray, float &t, Vector3f &param) const {
    bool exist = tryGetIntersectionPoint(ray, t);
    param = ray.getEndPoint(t);
    return exist;
}

bool IRayCastable::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    throw std::invalid_argument("Function tryGetIntersectionPoint not implemented.");
}
