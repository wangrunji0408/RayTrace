//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_SHAPE_H
#define INC_2RAYTRACE_SHAPE_H

#include "../Geometry/Geometry.h"

class Shape: public IRayCastable {
public:
    std::string name;
};

#endif //INC_2RAYTRACE_SHAPE_H
