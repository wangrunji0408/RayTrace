//
// Created by 王润基 on 2017/4/9.
//

#include <cmath>
#include "ModifiedPhong.h"

Color ModifiedPhong::getF(Vector3f const &l, Vector3f const &v, Vector3f const &n) {
    Vector3f r = Ray(Vector3f::zero, n).calcProjectionPoint(l);
    float ks = powf(r.dot(v), shininess);
    return diffuse + specular * ks;
}

ModifiedPhong::ModifiedPhong(const Color &diffuse, const Color &specular, float shininess) : Phong(diffuse, specular,
                                                                                                   shininess) {}
