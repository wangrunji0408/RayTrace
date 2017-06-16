//
// Created by 王润基 on 2017/4/8.
//

#include "Shape.h"
#include "3D/AxisBox.h"

Vector3f Shape::getUV(Vector3f const &point) const {
    return Vector3f::zero;
}

AxisBox Shape::getAABB() const {
    return AxisBox(Vector3f(-inf, -inf, -inf), Vector3f(inf, inf, inf));
}

float Shape::fastIntersect(Ray const& ray) const {
    auto aabb = getAABB();
    if(aabb.isInside(ray.getStartPoint()))
        return 0;
    float t;
    bool exist = aabb.tryGetIntersectionPoint(ray, t);
    return exist? t: inf;
}

float Shape::calcIntersect(Ray const &ray) const {
    auto info = IntersectInfo(ray);
    intersect(info);
    return info.success? info.t: inf;
}

bool Shape::testRayBlocked(Ray const &ray, float tmin) const {
    float t; Vector3f param;
    bool exist = tryGetIntersectionPoint(ray, t, param);
    return exist && t < tmin;
}
