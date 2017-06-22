//
// Created by 王润基 on 2017/4/8.
//

#include "Plane.h"
#include "../3D/AxisBox.h"

int Plane::intersectCount = 0;

float Plane::dist(Vector3f const &point) const {
    return normal.calcProjectionT(point);
}

bool Plane::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    intersectCount++;
    if (normal.getUnitDir().isVertical(ray.getUnitDir()))
        return false;
    float xs = normal.calcProjectionT(ray.getStartPoint());
    float xe = normal.calcProjectionT(ray.getEndPoint());
    t = xs / (xs - xe);
    return t > eps;
}

Plane::Plane(const Ray &normal) : normal(normal)
{
    trans = Transform::toZ01(normal.getStartPoint(), normal.getEndPoint());
}

Plane::Plane(Vector3f const &a, Vector3f const &b, Vector3f const &c) :
        normal(Ray(a, (b - a).det(c - a))) {}

bool Plane::isOnSurface(Vector3f const &point) const {
    return isZero(dist(point));
}

Vector3f Plane::getNormalVector(Vector3f const &param) const {
    return normal.getUnitDir();
}

Vector3f Plane::getUV(Vector3f const &point) const {
    return trans.apply_xy(point);
}

AxisBox Plane::getAABB() const {
    auto tinv = trans.inverse();
    Vector3f ps[] = {Point(-size, -size, -eps), Point(-size, size, -eps),
                     Point(size, -size, -eps), Point(size, size, -eps),
                     Point(-size, -size, eps), Point(-size, size, eps),
                     Point(size, -size, eps), Point(size, size, eps)};
    for(int i=0; i<8; ++i)
        ps[i] = tinv * ps[i];
    auto a = AxisBox(ps, 8);
    return a;
}

std::ostream &operator<<(std::ostream &os, const Plane &plane) {
    os << "[Plane normal: " << plane.normal << " size: " << plane.size << "]";
    return os;
}

