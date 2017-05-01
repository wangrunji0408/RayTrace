//
// Created by 王润基 on 2017/4/9.
//

#include "Triangle.h"
#include "../3D/AxisBox.h"

int Triangle::intersectCount = 0;

Triangle::Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c) :
        a(a), b(b), c(c) {}

bool Triangle::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    ++intersectCount;
    bool exist = getPlane().tryGetIntersectionPoint(ray, t);
    if(!exist)  return false;
    Vector3f p = ray.getEndPoint(t);
    return isOnSurface(p);
}

Plane Triangle::getPlane() const {
    return Plane(a, b, c);
}

inline float detx (Vector3f const& a, Vector3f const& b)
{
    return a.y * b.z - a.z * b.y;
}

bool Triangle::isOnSurface(Vector3f const &p) const {
//    Vector3f gc = calcGravityCoordinate(point);
//    return gc.x > -eps && gc.y > -eps && gc.z > -eps;
    float z = detx(b-a, p-a);
    float x = detx(c-b, p-b);
    float y = detx(a-c, p-c);
    return (x > 0 && y > 0 && z > 0) || (x < 0 && y < 0 && z < 0);
}

Vector3f Triangle::getNormalVectorOnSurface(Vector3f const &point) const {
    return getPlane().normal.getUnitDir();
}

Vector3f Triangle::calcGravityCoordinate(Vector3f const &p) const {
//    Vector3f detc = (b-a).det(p-a);
//    Vector3f deta = (c-b).det(p-b);
//    Vector3f detb = (a-c).det(p-c);
//    Vector3f deti = deta + detb + detc;
//    float ta = deta.dot(deti);
//    float tb = detb.dot(deti);
//    float tc = detc.dot(deti);
//    Vector3f t = Vector3f(ta, tb, tc);
//    return t / t.sum();
    // 注意到deta/detb/detc共线，因此只计算一个分量即可
    float z = detx(b-a, p-a);
    float x = detx(c-b, p-b);
    float y = detx(a-c, p-c);
    float sum = x + y + z;
    return Vector3f(x, y, z) / sum;
}

Triangle::Triangle(const Vector3f *points):
    Triangle(points[0], points[1], points[2]) {}

Vector3f Triangle::getUV(Vector3f const &point) const {
    float u = (b-a).dot(point - a) / (b-a).len2();
    float v = (c-a).dot(point - a) / (c-a).len2();
    return Vector3f(u,v,0);
}

AxisBox Triangle::getAABB() const {
    return AxisBox(&a, 3);
}
