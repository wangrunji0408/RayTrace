//
// Created by 王润基 on 2017/4/15.
//

#include <math.h>
#include "BlinnPhong.h"

BlinnPhong::BlinnPhong(const Color &diffuse, const Color &specular, float shininess) :
        Phong(diffuse, specular, shininess) {}

Color BlinnPhong::calcBRDF(Vector3f const &l, Vector3f const &v, Vector3f const &n) const {
    float dotnl = n.dot(l);
    if(dotnl < eps || n.dot(v) < eps)    return Color::zero;
    Vector3f h = ((l + v) / 2).norm();
    float ks = powf(n.dot(h), shininess);
    return diffuse * dotnl + specular * ks;
}


