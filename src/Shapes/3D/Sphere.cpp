//
// Created by 王润基 on 2017/4/8.
//

#include <cmath>
#include "Sphere.h"
#include "AxisBox.h"

int Sphere::intersectCount = 0;

Sphere::Sphere(const Vector3f &o, float r) : o(o), r(r) {}

bool Sphere::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    intersectCount++;
    float d = ray.calcDist(o);
    if(d > r + eps)
        return false;
    t = ray.calcProjectionT(o);
    float x = sqrtf(r * r - d * d);
    if(t > x - eps) t -= x;
    else if(t > -x - eps) t += x;
    else return false;
    return true;
}

bool Sphere::isInside(Vector3f const &point) const {
    return (point - o).len() < r - eps;
}

bool Sphere::isOnSurface(Vector3f const &point) const {
    return isZero((point - o).len() - r);
}

Vector3f Sphere::getNormalVector(Vector3f const &param) const {
    return (param - o).norm();
}

Sphere::Sphere(): o(Vector3f::zero), r(1) {

}

Vector3f Sphere::getUV(Vector3f const &point) const {
    auto r = (point - o).norm();
    float u = atan2f(r.y, r.x) / (float)M_PI / 2 + 0.5f;
    float v = asinf(r.z) / (float)M_PI + 0.5f;
    return Vector3f(u, v, 0);
}

AxisBox Sphere::getAABB() const {
    auto dr = Vector3f(r);
    return AxisBox(o - dr, o + dr);
}

bool Sphere::testRayBlocked(Ray const &ray, float tmin) const {
    return Shape3D::testRayBlocked(ray, tmin);
    if(tmin * tmin < (ray.getStartPoint() - o).len2())
        return false;
    float t;
    return tryGetIntersectionPoint(ray, t) && t < tmin;
}

Vector3f Sphere::sample() const {
    return o + Vector3f::getRandUnit() * r;
}

const Vector3f &Sphere::getO() const {
    return o;
}

