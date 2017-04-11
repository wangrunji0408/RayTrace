//
// Created by 王润基 on 2017/4/8.
//

#include "ParallelLight.h"

Light ParallelLight::illuminate(Vector3f const &point) const {
    Ray ray0 = Ray(point, ray.getUnitDir());
    Color color0 = ray.calcProjectionT(point) > 0? color: Vector3f::zero;
    return Light(ray0, color0);
}
