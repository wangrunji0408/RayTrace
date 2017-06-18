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

bool Triangle::edge = false;

bool Triangle::isOnSurface(Vector3f const &p) const {
//    Vector3f gc = calcGravityCoordinate(p);
//    return gc.x > -eps && gc.y > -eps && gc.z > -eps;
    float z = detx(b-a, p-a);
    float x = detx(c-b, p-b);
    float y = detx(a-c, p-c);
    if(edge)
        // 含边界，三角网格缝隙处会重合，使用法向插值时会出现花纹现象
        return (x > -eps && y > -eps && z > -eps)
               || (x < eps && y < eps && z < eps);
    else
        // 不含边界，三角网格缝隙处会漏过去，ParameterSurface出现黑点
        return (x >= 0 && y >= 0 && z >= 0)
               || (x <= 0 && y <= 0 && z <= 0);
}

Vector3f Triangle::getNormalVector(Vector3f const &param) const {
    return getPlane().normal.getUnitDir();
}

Vector3f Triangle::calcGravityCoordinate(Vector3f const &p) const {
    // 注意到deta/detb/detc共线，因此只计算一个分量即可
    float z = detx(b-a, p-a);
    float x = detx(c-b, p-b);
    float y = detx(a-c, p-c);
    float sum = x + y + z;
    if(fabs(sum) > eps)
        return Vector3f(x, y, z) / sum;
    // 如果sum为0，则用原方法计算
    Vector3f detc = (b-a).det(p-a);
    Vector3f deta = (c-b).det(p-b);
    Vector3f detb = (a-c).det(p-c);
    Vector3f deti = deta + detb + detc;
    float ta = deta.dot(deti);
    float tb = detb.dot(deti);
    float tc = detc.dot(deti);
    Vector3f t = Vector3f(ta, tb, tc);
    return t / t.sum();
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

bool Triangle::testRayBlocked(Ray const &ray, float tmin) const {
    ++intersectCount;
    float t;
    bool exist = getPlane().tryGetIntersectionPoint(ray, t);
    if(!exist || t >= tmin)  return false;
    Vector3f p = ray.getEndPoint(t);
    return isOnSurface(p);
}
