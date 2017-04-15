//
// Created by 王润基 on 2017/4/9.
//

#include <cmath>
#include "Phong.h"

Phong::Phong(const Color &diffuse, const Color &specular, float shininess) :
        diffuse(diffuse), specular(specular), shininess(shininess) {}

Color Phong::calcBRDF(Vector3f const &l, Vector3f const &v, Vector3f const &n) const {
    float dotnl = n.dot(l);
    if(dotnl < eps || n.dot(v) < eps)    return Color::zero;
    Vector3f r = Ray(Vector3f::zero, n).calcProjectionPoint(l);
    float ks = powf(r.dot(v), shininess);
    return diffuse * dotnl + specular * ks;
}


