//
// Created by 王润基 on 2017/4/8.
//

#include "../../Geometry/Ray.h"
#include "Shape2D.h"

bool Shape2D::tryGetIntersectionInfo(const Ray &ray, float &t, Vector3f &point, Vector3f &normal) const {
    if(!tryGetIntersectionPoint(ray, t))
        return false;
    point = ray.getEndPoint(t);
    normal = getNormalVectorOnSurface(point);
    return true;
}
