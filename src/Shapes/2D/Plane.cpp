//
// Created by 王润基 on 2017/4/8.
//

#include "Plane.h"
#include "../3D/AxisBox.h"

int Plane::intersectCount = 0;

float Plane::dist(Vector3f const &point) const {
    return normal.calcProjectionT(point);
}

bool Plane::tryGetIntersectionPoint(Ray const &ray, float &t, Vector3f &param) const {
    intersectCount++;
    t = intersectWithPlane(normal, ray);
    if(t == inf)    return false;
    param = trans.apply_xy(ray.getEndPoint(t));
    return true;
}

Plane::Plane(const Ray &normal) : normal(normal)
{
    trans = Transform::toZ01(normal.getStartPoint(), normal.getEndPoint());
    trans_inv = trans.inverse();
}

Plane::Plane(Vector3f const &a, Vector3f const &b, Vector3f const &c) :
        normal(Ray(a, (b - a).det(c - a))) {}

bool Plane::isOnSurface(Vector3f const &point) const {
    return isZero(dist(point));
}

Vector3f Plane::getNormalVector(Vector3f const &param) const {
    return normal.getUnitDir();
}

Vector3f Plane::getUV(Vector3f const &param) const {
    return param;
}

AxisBox Plane::getAABB() const {
    Vector3f ps[] = {Point(-size, -size, -eps), Point(-size, size, -eps),
                     Point(size, -size, -eps), Point(size, size, -eps),
                     Point(-size, -size, eps), Point(-size, size, eps),
                     Point(size, -size, eps), Point(size, size, eps)};
    for(int i=0; i<8; ++i)
        ps[i] = trans_inv * ps[i];
    auto a = AxisBox(ps, 8);
    return a;
}

Vector3f Plane::sample() const {
    float x = rand01();
    return Vector3f();
}

float Plane::intersectWithPlane(Ray const& normal, const Ray &ray) {
    if (normal.getUnitDir().isVertical(ray.getUnitDir()))
        return inf;
    float xs = normal.calcProjectionT(ray.getStartPoint());
    float xe = normal.calcProjectionT(ray.getEndPoint());
    float t = xs / (xs - xe);
    return t > eps? t: inf;
}

std::ostream &operator<<(std::ostream &os, const Plane &plane) {
    os << "[Plane normal: " << plane.normal << " size: " << plane.size << "]";
    return os;
}

