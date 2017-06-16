//
// Created by 王润基 on 2017/4/8.
//

#include "../../Geometry/Ray.h"
#include "Shape2D.h"

bool Shape2D::tryGetIntersectionInfo(const Ray &ray, float &t, Vector3f &param, Vector3f &normal) const {
    if(!tryGetIntersectionPoint(ray, t, param))
        return false;
    normal = getNormalVector(param);
    return true;
}
