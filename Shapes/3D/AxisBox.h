//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_AXISBOX_H
#define INC_2RAYTRACE_AXISBOX_H


#include <ostream>
#include "Box.h"

class AxisBox: public Shape3D {
public:
    Vector3f minp, maxp;
    static int intersectCount;
public:
    AxisBox();
    AxisBox(const Vector3f &minp, const Vector3f &maxp);
    AxisBox(const Vector3f *points, size_t n);
    Vector3f getNormalVectorOnSurface(Vector3f const &point) const override;
    float getSurfaceArea () const;
    AxisBox merge (AxisBox const& other);
    Vector3f getMidPoint () const;
    bool isInside(Vector3f const &point) const override;
    bool isOnSurface(Vector3f const &point) const override;
    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;
    friend std::ostream &operator<<(std::ostream &os, const AxisBox &box);

    float fastIntersect(Ray const &ray) const override;
    AxisBox getAABB() const override;
};


#endif //INC_2RAYTRACE_AXISBOX_H
