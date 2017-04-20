//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_GRAPHIC_H
#define INC_2RAYTRACE_GRAPHIC_H

#include "Vector3.h"
#include "Ray.h"
#include "IRayCastable.h"

typedef Vector3f Point;

const float eps = 1e-5;
const float inf = 1e36;
inline bool isZero (float x);
inline bool isEqual (float x, float y);
inline bool isBetween(float x, float min, float max);

inline bool isZero(float x) {
    return x < eps && x > -eps;
}

inline bool isBetween(float x, float min, float max) {
    return x > min + eps && x < max - eps;
}

inline bool isBetweenOrEqual(float x, float min, float max) {
    return x > min - eps && x < max + eps;
}

inline bool isEqual(float x, float y) {
    return isZero(x - y);
}

inline float rand01 ()
{
    return (float)rand() / RAND_MAX;
}

template <class T>
inline void updateMin (T& x, T const& y)
{
    x = std::min(x, y);
}

template <class T>
inline void updateMax (T& x, T const& y)
{
    x = std::max(x, y);
}

#endif //INC_2RAYTRACE_GRAPHIC_H
