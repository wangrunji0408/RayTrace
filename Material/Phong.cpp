//
// Created by 王润基 on 2017/4/9.
//

#include <cmath>
#include "Phong.h"

Color Phong::getF(Vector3f const &ll, Vector3f const &vv, Vector3f const &nn) {
    auto l = ll.norm(), v = vv.norm(), n = nn.norm();
    if(n.dot(l) < eps || n.dot(v) < eps)    return Color::zero;
    Vector3f r = Ray(Vector3f::zero, n).calcProjectionPoint(l);
    float ks = powf(r.dot(v), shininess) / n.dot(l);
    return diffuse + specular * ks;
}

Phong::Phong(const Color &diffuse, const Color &specular, float shininess) :
        diffuse(diffuse), specular(specular), shininess(shininess) {}


