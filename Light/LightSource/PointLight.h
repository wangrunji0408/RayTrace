//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_POINTLIGHT_H
#define INC_2RAYTRACE_POINTLIGHT_H


#include "LightSource.h"

class PointLight: public LightSource {
    Vector3f pos;
    Color color;
public:
    Light illuminate(Vector3f const &point) const override;
};


#endif //INC_2RAYTRACE_POINTLIGHT_H
