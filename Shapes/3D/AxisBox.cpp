//
// Created by 王润基 on 2017/4/8.
//

#include <algorithm>
#include <cmath>
#include "AxisBox.h"

bool AxisBox::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    ++intersectCount;
    Vector3f const& s = ray.getStartPoint();
    Vector3f const& d = ray.getUnitDir();
    auto tmin = (minp - s) / d;
    auto tmax = (maxp - s) / d;
    if(tmin.x > tmax.x) std::swap(tmin.x, tmax.x);
    if(tmin.y > tmax.y) std::swap(tmin.y, tmax.y);
    if(tmin.z > tmax.z) std::swap(tmin.z, tmax.z);
    float min = std::max(std::max(tmin.x, tmin.y), tmin.z);
    float max = std::min(std::min(tmax.x, tmax.y), tmax.z);
    t = min;
    return min <= max && t > eps;
}

bool AxisBox::isInside(Vector3f const &p) const {
    bool xb = isBetween(p.x, minp.x, maxp.x);
    bool yb = isBetween(p.y, minp.y, maxp.y);
    bool zb = isBetween(p.z, minp.z, maxp.z);
    return xb && yb && zb;
}

bool AxisBox::isOnSurface(Vector3f const &p) const {
    bool xb = isBetween(p.x, minp.x, maxp.x);
    bool yb = isBetween(p.y, minp.y, maxp.y);
    bool zb = isBetween(p.z, minp.z, maxp.z);
    bool xe = isEqual(p.x, minp.x) || isEqual(p.x, maxp.x);
    bool ye = isEqual(p.y, minp.y) || isEqual(p.y, maxp.y);
    bool ze = isEqual(p.z, minp.z) || isEqual(p.z, maxp.z);
    bool isNotOutside = (xb || xe) && (yb || ye) && (zb || ze);
    bool isInside = xb && yb && zb;
    return isNotOutside && !isInside;
}

Vector3f AxisBox::getNormalVectorOnSurface(Vector3f const &p) const {
    bool xb = isBetweenOrEqual(p.x, minp.x, maxp.x);
    bool yb = isBetweenOrEqual(p.y, minp.y, maxp.y);
    bool zb = isBetweenOrEqual(p.z, minp.z, maxp.z);
    if(xb && yb)
    {
        if(isEqual(p.z, minp.z))    return Vector3f(0, 0, -1);
        if(isEqual(p.z, maxp.z))    return Vector3f(0, 0, 1);
    }
    if(xb && zb)
    {
        if(isEqual(p.y, minp.y))    return Vector3f(0, -1, 0);
        if(isEqual(p.y, maxp.y))    return Vector3f(0, 1, 0);
    }
    if(yb && zb)
    {
        if(isEqual(p.x, minp.x))    return Vector3f(-1, 0, 0);
        if(isEqual(p.x, maxp.x))    return Vector3f(1, 0, 0);
    }
    std::cerr << p << std::endl;
    throw std::exception();
//    return Vector3f::zero;  // should not reach here
}

AxisBox::AxisBox(const Vector3f &a, const Vector3f &b) :
        minp(Vector3f(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z))),
        maxp(Vector3f(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z))) {}

AxisBox::AxisBox() {}

AxisBox::AxisBox(const Vector3f *points, size_t n) {
    minp = Point(inf, inf, inf);
    maxp = -Point(inf, inf, inf);
    for(int i=0; i<n; ++i){
        Point const& p = points[i];
        updateMin(minp.x, p.x);
        updateMin(minp.y, p.y);
        updateMin(minp.z, p.z);
        updateMax(maxp.x, p.x);
        updateMax(maxp.y, p.y);
        updateMax(maxp.z, p.z);
    }
//    minp += -Point(eps, eps, eps);
//    maxp += Point(eps, eps, eps);
}

float AxisBox::getSurfaceArea() const {
    auto d = maxp - minp;
    return (d.x * d.y + d.y * d.z + d.z * d.x) * 2;
}

Vector3f AxisBox::getMidPoint() const {
    return (minp + maxp) / 2;
}

AxisBox AxisBox::getAABB() const {
    return *this;
}

float AxisBox::fastIntersect(Ray const &ray) const {
    if(isInside(ray.getStartPoint()))
        return 0;
    float t;
    bool exist = tryGetIntersectionPoint(ray, t);
    return exist? t: inf;
}

std::ostream &operator<<(std::ostream &os, const AxisBox &box) {
    os << "[AxisBox minp: " << box.minp << " maxp: " << box.maxp << "]";
    return os;
}

AxisBox AxisBox::merge(AxisBox const& other) {
    AxisBox c;
    c.minp.x = std::min(minp.x, other.minp.x);
    c.minp.y = std::min(minp.y, other.minp.y);
    c.minp.z = std::min(minp.z, other.minp.z);
    c.maxp.x = std::max(maxp.x, other.maxp.x);
    c.maxp.y = std::max(maxp.y, other.maxp.y);
    c.maxp.z = std::max(maxp.z, other.maxp.z);
    return c;
}

int AxisBox::intersectCount = 0;