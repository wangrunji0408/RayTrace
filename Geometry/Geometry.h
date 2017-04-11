//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_GRAPHIC_H
#define INC_2RAYTRACE_GRAPHIC_H

#include "Vector3.h"
#include "Ray.h"
#include "IRayCastable.h"

const float eps = 1e-6;
const float inf = 1e28;
inline bool isZero (float x);
inline bool isEqual (float x, float y);
inline bool isBetween(float x, float min, float max);

inline bool isZero(float x) {
    return x < eps && x > -eps;
}

inline bool isBetween(float x, float min, float max) {
    return x > min + eps && x < max - eps;
}

inline bool isEqual(float x, float y) {
    return isZero(x - y);
}

#endif //INC_2RAYTRACE_GRAPHIC_H
