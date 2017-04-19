//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_SPHEREMAP_H
#define INC_2RAYTRACE_SPHEREMAP_H


#include "UVMap.h"
#include "../Geometry/Ray.h"

class SphereMap: public UVMap {
    Ray up;
public:
    Vector3f getUV(Vector3f const &point) const override;
};


#endif //INC_2RAYTRACE_SPHEREMAP_H
