//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_GRAPHIC_H
#define INC_2RAYTRACE_GRAPHIC_H

#include <memory>
#include <limits>
#include "Vector3.h"
#include "Ray.h"
#include "IRayCastable.h"

using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;

typedef Vector3f Point;

const float eps = std::numeric_limits<float>::epsilon() * 8;
const float inf = std::numeric_limits<float>::max() / 8;
inline bool isZero (float x);
inline bool isEqual (float x, float y);
inline bool isBetween(float x, float min, float max);

inline bool isZero(float x) {
    return std::abs(x) < eps;
//    return x < eps && x > -eps;
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

inline float rsqrtf( float number )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking（对浮点数的邪恶位元hack）
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?（这他妈的是怎么回事？）
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次迭代）
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）
    y  = y * ( threehalfs - ( x2 * y * y ) );
    return y;
}

#endif //INC_2RAYTRACE_GRAPHIC_H
