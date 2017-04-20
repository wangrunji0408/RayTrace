//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_AXISBOX_H
#define INC_2RAYTRACE_AXISBOX_H


#include <ostream>
#include "Box.h"

class AxisBox: public Box {
public:
    Vector3f minp, maxp;
protected:
public:
    friend std::ostream &operator<<(std::ostream &os, const AxisBox &box);

protected:
    static float getIntersectionT1D (float x, bool dirMinus, float min, float max);

public:
    AxisBox();
    AxisBox(const Vector3f &minp, const Vector3f &maxp);
    Vector3f getNormalVectorOnSurface(Vector3f const &point) const override;
    bool isInside(Vector3f const &point) const override;
    bool isOnSurface(Vector3f const &point) const override;
    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;
};


#endif //INC_2RAYTRACE_AXISBOX_H
