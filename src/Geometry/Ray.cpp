//
// Created by 王润基 on 2017/4/8.
//

#include "Ray.h"

Ray::Ray(const Vector3f &start, const Vector3f &direction) :
        start(start), unitdir(direction.norm()) {}

float Ray::calcProjectionT(Vector3f const &p) const {
    return (p - start).dot(unitdir);
}

Vector3f Ray::getEndPoint(float t) const {
    return start + unitdir * t;
}

Vector3f Ray::getEndPoint() const {
    return start + unitdir;
}

Vector3f Ray::getStartPoint() const {
    return start;
}

Vector3f Ray::getUnitDir() const {
    return unitdir;
}

float Ray::calcDist(Vector3f const &p) const {
    return (p - start).det(unitdir).len();
}

Ray Ray::fromTo(Vector3f const &from, Vector3f const &to) {
    return Ray(from, to - from);
}

Vector3f Ray::calcSymmetryPoint(Vector3f const &p) const {
    return calcProjectionPoint(p) * 2 - p;
}

Vector3f Ray::calcProjectionPoint(Vector3f const &p) const {
    return getEndPoint(calcProjectionT(p));
}

Ray::Ray(): start(Vector3f::zero), unitdir(Vector3f(1,0,0)) {}

Ray Ray::raw(Vector3f const& start, Vector3f const& unitdir) {
    auto ray = Ray();
    ray.start = start;
    ray.unitdir = unitdir;
    return ray;
}

std::ostream &operator<<(std::ostream &out, Ray const &ray) {
    return out << "[Ray s = " << ray.start << " d = " << ray.unitdir << "]";
}
