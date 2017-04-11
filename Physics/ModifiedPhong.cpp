//
// Created by 王润基 on 2017/4/9.
//

#include <cmath>
#include "ModifiedPhong.h"

float ModifiedPhong::getF(Vector3f const &in, Vector3f const &out, Ray const &face) {
    Vector3f const& o = face.getStartPoint();
    Vector3f v = out - o;
    Vector3f r = face.calcProjectionPoint(in) - o;
    float ks = powf(r.dot(v), shiness);
    return pd + ps * ks;
}

ModifiedPhong::ModifiedPhong(float pd, float ps, float shiness) : Phong(pd, ps, shiness) {}
