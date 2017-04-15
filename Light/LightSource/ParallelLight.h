//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_PARALLELLIGHT_H
#define INC_2RAYTRACE_PARALLELLIGHT_H


#include "LightSource.h"
#include "../../Geometry/Ray.h"

class ParallelLight: public LightSource {
    Ray ray;
    Color color;
public:
    ParallelLight(const Ray &ray, const Color &color);

    const Ray &getRay() const;

    void setRay(const Ray &ray);

    const Color &getColor() const;

    void setColor(const Color &color);

    Light illuminate(Vector3f const &point) const override;
};


#endif //INC_2RAYTRACE_PARALLELLIGHT_H
