//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_PARALLELLIGHT_H
#define INC_2RAYTRACE_PARALLELLIGHT_H


#include "LightSource.h"
#include "../../Geometry/Ray.h"

class ParallelLight: public LightSource {
public:
    Ray ray;
    Color color;

    Light illuminate(Vector3f const &point) const override;
};


#endif //INC_2RAYTRACE_PARALLELLIGHT_H
