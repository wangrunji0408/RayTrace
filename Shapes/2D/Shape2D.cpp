//
// Created by 王润基 on 2017/4/8.
//

#include "../../Geometry/Ray.h"
#include "Shape2D.h"

void Shape2D::intersect(IntersectInfo &info) const {
    if(info.testBlockT != 0) {
        info.success = testRayBlocked(info.ray, info.testBlockT);
        return;
    }
    info.success = tryGetIntersectionPoint(info.ray, info.t, info.param);
    if(info.needNormal)
        info.normal = getNormalVector(info.param);
    if(info.needUV)
        info.uv = getUV(info.param);
}
