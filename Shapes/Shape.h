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
public:
    std::string name;
};

#endif //INC_2RAYTRACE_SHAPE_H
