//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_LAMBERTAIN_H
#define INC_2RAYTRACE_LAMBERTAIN_H


#include "BRDF.h"

class Lambertain: public BRDF {
public:
    float albedo;
public:
    Lambertain(float albedo);

    float getF(Vector3f const &in, Vector3f const &out, Ray const &face) override;
};


#endif //INC_2RAYTRACE_LAMBERTAIN_H
