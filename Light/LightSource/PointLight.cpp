//
// Created by 王润基 on 2017/4/8.
//

#include "PointLight.h"

Light PointLight::illuminate(Vector3f const &point) const {
    Vector3f d = point - pos;
    auto omiga = 1 / d.len2();
    return Light(Ray(point, d), color * omiga);
}
