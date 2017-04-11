//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_BRDF_H
#define INC_2RAYTRACE_BRDF_H


#include "../Geometry/Geometry.h"

class BRDF {
public:
    virtual float getF (Vector3f const& in, Vector3f const& out, Ray const& face) = 0;
};


#endif //INC_2RAYTRACE_BRDF_H
