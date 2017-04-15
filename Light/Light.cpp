//
// Created by 王润基 on 2017/4/8.
//

#include "Light.h"

Vector3f Light::getUnitDir() const {
    return ray.getUnitDir();
}

std::ostream &operator<<(std::ostream &os, const Light &light) {
    os << "[Light ray: " << light.ray << " color: " << light.color << "]";
    return os;
}
