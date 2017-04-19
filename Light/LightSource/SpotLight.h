//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_SPOTLIGHT_H
#define INC_2RAYTRACE_SPOTLIGHT_H


#include "LightSource.h"

class SpotLight: public LightSource {
    Ray ray;
    float angle;
    float edgeAngle;
public:
    Light illuminate(Vector3f const &point) const override;
    SpotLight(Color const &color, const Ray &ray, float angle, float edgeAngle = 0);
    bool inRange (Vector3f const &point) const;
};


#endif //INC_2RAYTRACE_SPOTLIGHT_H
