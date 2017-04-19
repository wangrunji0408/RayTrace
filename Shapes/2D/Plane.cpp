//
// Created by 王润基 on 2017/4/8.
//

#include "Plane.h"

float Plane::dist(Vector3f const &point) const {
    return normal.calcProjectionT(point);
}

bool Plane::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    if (normal.getUnitDir().isVertical(ray.getUnitDir()))
        return false;
    float xs = normal.calcProjectionT(ray.getStartPoint());
    float xe = normal.calcProjectionT(ray.getEndPoint());
    t = xs / (xs - xe);
    return t > eps;
}

Plane::Plane(const Ray &normal) : normal(normal) {}

Plane::Plane(Vector3f const &a, Vector3f const &b, Vector3f const &c) :
        normal(Ray(a, (b - a).det(c - a))) {}

bool Plane::isOnSurface(Vector3f const &point) const {
    return isZero(dist(point));
}

Vector3f Plane::getNormalVectorOnSurface(Vector3f const &point) const {
    return normal.getUnitDir();
}

Vector3f Plane::getUV(Vector3f const &point) const {
    Point const& p = normal.getStartPoint();
    Ray rayu = Ray(p, normal.getUnitDir().det(Vector3f(1,0,0)));
    Ray rayv = Ray(p, normal.getUnitDir().det(rayu.getUnitDir()));
    float u = rayu.calcProjectionT(point - p);
    float v = rayv.calcProjectionT(point - p);
    return Vector3f(u,v,0);
}

