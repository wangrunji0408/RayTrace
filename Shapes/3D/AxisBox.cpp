//
// Created by 王润基 on 2017/4/8.
//

#include <algorithm>
#include "AxisBox.h"

bool AxisBox::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    Vector3f const& s = ray.getStartPoint();
    Vector3f const& d = ray.getUnitDir();
    float tx = getIntersectionT1D(s.x, d.x < 0, minp.x, maxp.x);
    float ty = getIntersectionT1D(s.y, d.y < 0, minp.y, maxp.y);
    float tz = getIntersectionT1D(s.z, d.z < 0, minp.z, maxp.z);
    if(tx == inf || ty == inf || tz == inf)
        return false;
    tx /= d.x, ty /= d.y, tz /= d.z;
    t = std::min(std::min(tx, ty), tz);
    // 检测在不在表面？？
    return true;
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
    if(dirMinus)
        x = -x, std::swap(min, max), min = -min, max = -max;
    if(x < min + eps)       return min - x;
    else if(x < max + eps)  return max - x;
    else                    return inf;
}

Vector3f AxisBox::getNormalVectorOnSurface(Vector3f const &p) const {
    bool xb = isBetween(p.x, minp.x, maxp.x);
    bool yb = isBetween(p.y, minp.y, maxp.y);
    bool zb = isBetween(p.z, minp.z, maxp.z);
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
    return Vector3f::zero;  // should not reach here
}

