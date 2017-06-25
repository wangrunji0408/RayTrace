//
// Created by wangrunji0408 on 2017/6/22.
//

#ifndef INC_2RAYTRACE_RECTLIGHT_H
#define INC_2RAYTRACE_RECTLIGHT_H


#include "LightSource.h"
#include "../../Shapes/2D/Plane.h"

class RectLight: public LightSource {
    Plane plane;
public:
    int sampleSize = 4;
public:
    RectLight(Color const& color, const Ray &normal, float size);
    Light illuminate(Vector3f const &point) const override;
    std::vector<Light> illuminates(Vector3f const &point) const override;
    Ray sample() const override;
};


#endif //INC_2RAYTRACE_RECTLIGHT_H
