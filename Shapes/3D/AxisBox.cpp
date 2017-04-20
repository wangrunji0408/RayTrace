//
// Created by 王润基 on 2017/4/8.
//

#include <algorithm>
#include <cmath>
#include "AxisBox.h"

bool AxisBox::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    Vector3f const& s = ray.getStartPoint();
    Vector3f const& d = ray.getUnitDir();
    float tx = getIntersectionT1D(s.x, d.x < 0, minp.x, maxp.x);
    float ty = getIntersectionT1D(s.y, d.y < 0, minp.y, maxp.y);
    float tz = getIntersectionT1D(s.z, d.z < 0, minp.z, maxp.z);
    if(tx == inf || ty == inf || tz == inf)
        return false;
    tx /= fabs(d.x), ty /= fabs(d.y), tz /= fabs(d.z);
    t = std::max(std::max(tx, ty), tz);
    return t > -eps && isOnSurface(ray.getEndPoint(t));
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

float AxisBox::getIntersectionT1D(float x, bool dirMinus, float min, float max) {
    if(dirMinus)    return -max + x;
    return min - x;
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

std::ostream &operator<<(std::ostream &os, const AxisBox &box) {
    os << "[AxisBox minp: " << box.minp << " maxp: " << box.maxp << "]";
    return os;
}

