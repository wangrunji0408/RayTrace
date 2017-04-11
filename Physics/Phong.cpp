//
// Created by 王润基 on 2017/4/9.
//

#include <cmath>
#include "Phong.h"

float Phong::getF(Vector3f const &in, Vector3f const &out, Ray const &face) {
    Vector3f const& o = face.getStartPoint();
    Vector3f const& n = face.getUnitDir();
    Vector3f l = in - o;
    Vector3f v = out - o;
    Vector3f r = face.calcProjectionPoint(in) - o;
    float ks = powf(r.dot(v), shiness) / n.dot(l);
    return pd + ps * ks;
}

Phong::Phong(float pd, float ps, float shiness) : pd(pd), ps(ps), shiness(shiness) {}


