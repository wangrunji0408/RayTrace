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
    throw std::invalid_argument("Function tryGetIntersectionPoint is not implemented.");
}

void IRayCastable::intersect(IntersectInfo &info) const {
    info.success = tryGetIntersectionPoint(info.ray, info.t, info.param);
    if(!info.success)   return;
    if(info.testBlockT != 0)
    {
        info.success = info.t < info.testBlockT;
        return;
    }
    if(info.needNormal || info.needParam || info.needObject)
        throw std::runtime_error("Function intersect is not implemented. Needs can not be satisfied.");
}
