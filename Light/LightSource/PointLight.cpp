//
// Created by 王润基 on 2017/4/8.
//

#include "PointLight.h"

Light PointLight::illuminate(Vector3f const &point) const {
    Vector3f d = point - pos;
    auto omiga = 1 / d.len2();
    return Light(Ray(point, d), color * omiga);
}

PointLight::PointLight(const Vector3f &pos, const Color &color) : pos(pos), color(color) {}

const Vector3f &PointLight::getPos() const {
    return pos;
}

void PointLight::setPos(const Vector3f &pos) {
    PointLight::pos = pos;
}

const Color &PointLight::getColor() const {
    return color;
}

void PointLight::setColor(const Color &color) {
    PointLight::color = color;
}

PointLight::PointLight(): pos(Vector3f::zero), color(Vector3f::zero) {}

std::ostream &operator<<(std::ostream &os, const PointLight &light) {
    os << "[PointLight pos: " << light.pos << " color: " << light.color << "]";
    return os;
}
