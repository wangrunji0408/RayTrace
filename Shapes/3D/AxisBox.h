//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_AXISBOX_H
#define INC_2RAYTRACE_AXISBOX_H


#include "Box.h"

class AxisBox: public Box {
    Vector3f minp, maxp;

    static float getIntersectionT1D (float x, bool dirMinus, float min, float max);

public:
    Vector3f getNormalVectorOnSurface(Vector3f const &point) const override;
    bool isInside(Vector3f const &point) const override;
    bool isOnSurface(Vector3f const &point) const override;
    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;
};


#endif //INC_2RAYTRACE_AXISBOX_H
