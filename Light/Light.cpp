//
// Created by 王润基 on 2017/4/8.
//

#include "Light.h"

Vector3f Light::getUnitDir() const {
    return (end - begin).norm();
}

Light::Light(const Vector3f &begin, const Vector3f &end, const Color &color) : begin(begin), end(end), color(color) {}

float Light::len() const {
    return (end - begin).len();
}

Ray Light::getRay() const {
    return Ray(begin, end - begin);
}

std::ostream &operator<<(std::ostream &os, const Light &light) {
    os << "[Light begin: " << light.begin << " end: " << light.end << " color: " << light.color << "]";
    return os;
}
