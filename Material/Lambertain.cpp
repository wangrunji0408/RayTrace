//
// Created by 王润基 on 2017/4/9.
//

#include <math.h>
#include "Lambertain.h"

Lambertain::Lambertain(Color color) : diffuse(color) {}

Color Lambertain::calcBRDF(Vector3f const &l, Vector3f const &v, Vector3f const &n) const {
    if(l.dot(n) < eps || v.dot(n) < eps)   return Vector3f::zero;
    return diffuse / (float)M_PI;
}


