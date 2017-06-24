//
// Created by 王润基 on 2017/4/8.
//

#include "IRayCastable.h"
#include "Geometry.h"
#include "../Shapes/3D/AxisBox.h"

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
    info.object = this;
    if(!info.success)   return;
    if(info.testBlockT != 0)
    {
        info.success = info.t < info.testBlockT;
        return;
    }
    if(info.needNormal || info.needParam)
        throw std::runtime_error("Function intersect is not implemented. Needs can not be satisfied.");
}

AxisBox IRayCastable::getAABB() const {
    return AxisBox(Vector3f(-inf, -inf, -inf), Vector3f(inf, inf, inf));
}

float IRayCastable::fastIntersect(Ray const& ray) const {
    auto aabb = getAABB();
    if(aabb.isInside(ray.getStartPoint()))
        return 0;
    float t;
    bool exist = aabb.tryGetIntersectionPoint(ray, t);
    return exist? t: inf;
}

float IRayCastable::calcIntersect(Ray const &ray) const {
    auto info = IntersectInfo(ray);
    intersect(info);
    return info.success? info.t: inf;
}

bool IRayCastable::testRayBlocked(Ray const &ray, float tmin) const {
    return calcIntersect(ray) < tmin;
}

Vector3f IRayCastable::getNormalVector(Vector3f const &param) const {
    throw std::invalid_argument("Function getNormalVector is not implemented.");
}

Vector3f IRayCastable::sample() const {
    throw std::invalid_argument("Function sample is not implemented.");
}
