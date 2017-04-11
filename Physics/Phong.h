//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_PHONG_H
#define INC_2RAYTRACE_PHONG_H


#include "BRDF.h"

class Phong: public BRDF {
public:
    float pd, ps, shiness;
public:
    Phong(float pd, float ps, float shiness);

    float getF(Vector3f const &in, Vector3f const &out, Ray const &face) override;
};


#endif //INC_2RAYTRACE_PHONG_H
