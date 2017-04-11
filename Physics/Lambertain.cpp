//
// Created by 王润基 on 2017/4/9.
//

#include <math.h>
#include "Lambertain.h"

Lambertain::Lambertain(float albedo) : albedo(albedo) {}

float Lambertain::getF(Vector3f const &in, Vector3f const &out, Ray const &face) {
    return albedo / (float)M_PI;
}


