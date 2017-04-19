//
// Created by 王润基 on 2017/4/19.
//

#include <cmath>
#include "SphereMap.h"

Vector3f SphereMap::getUV(Vector3f const &point) const {
    auto r = (point - pos).norm();
    float u = atan2f(r.y, r.x) / (float)M_PI / 2 + 0.5f;
    float v = asinf(r.z) / (float)M_PI + 0.5f;
    return Vector3f(u, v, 0);
}
