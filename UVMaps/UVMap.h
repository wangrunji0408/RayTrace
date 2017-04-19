//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_UVMAP_H
#define INC_2RAYTRACE_UVMAP_H


#include "../../Geometry/Vector3.h"

class UVMap {
public:
    virtual Vector3f getUV (Vector3f const& point) const;
};

#endif //INC_2RAYTRACE_UVMAP_H
