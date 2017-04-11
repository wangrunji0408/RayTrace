//
// Created by 王润基 on 2017/4/8.
//

#include <cmath>
#include "Sphere.h"

Sphere::Sphere(const Vector3f &o, float r) : o(o), r(r) {}

bool Sphere::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    float d = ray.calcDist(o);
    if(d > r + eps)
        return false;
    t = ray.calcProjectionT(o);
    if(d < r - eps) // d < r
        t -= sqrt(r * r - d * d);
    return true;
}

bool Sphere::isInside(Vector3f const &point) const {
    return (point - o).len() < r - eps;
}

bool Sphere::isOnSurface(Vector3f const &point) const {
    return isZero((point - o).len() - r);
}

Vector3f Sphere::getNormalVectorOnSurface(Vector3f const &point) const {
    return (point - o).norm();
}

