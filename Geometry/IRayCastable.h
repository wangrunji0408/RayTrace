//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_IRAYCASTABLE_H
#define INC_2RAYTRACE_IRAYCASTABLE_H

#include "Vector3.h"
#include "Ray.h"
#include "IntersectInfo.h"

class AxisBox;

class IRayCastable {
public:
    virtual bool tryGetIntersectionPoint(Ray const &ray, float& t) const;
    virtual bool tryGetIntersectionPoint(Ray const &ray, float& t, Vector3f& param) const;
    virtual void intersect (IntersectInfo &info) const;
    virtual float fastIntersect (Ray const& ray) const; // 与包围盒求交
    float calcIntersect (Ray const& ray) const;         // 与本体求交
    virtual bool testRayBlocked (Ray const& ray, float tmin) const;
    virtual AxisBox getAABB() const;
    virtual Vector3f getNormalVector(Vector3f const &param) const;
    virtual Vector3f sample () const; // return Param
};

#endif //INC_2RAYTRACE_IRAYCASTABLE_H
