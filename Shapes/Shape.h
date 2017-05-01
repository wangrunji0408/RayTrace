//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_SHAPE_H
#define INC_2RAYTRACE_SHAPE_H

#include "../Geometry/Geometry.h"
#include "../UVMaps/UVMap.h"

class AxisBox;

class Shape: public IRayCastable, public UVMap {
public:
    Vector3f getUV(Vector3f const &point) const override;
    virtual float fastIntersect (Ray const& ray) const;
    float calcIntersect (Ray const& ray) const;
    virtual bool testRayBlocked (Ray const& ray, float tmin) const;
    virtual AxisBox getAABB() const;
public:
    std::string name;
};

#endif //INC_2RAYTRACE_SHAPE_H
