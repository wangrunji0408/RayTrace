//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_PATHTRACER_H
#define INC_2RAYTRACE_PATHTRACER_H


#include "Renderer.h"
#include "RayTracer.h"

class PathTracer: public RayTracer {
public:
    int times = 10;
protected:
    Color renderRay(Ray const &ray, int depth, Color weight) const override;
public:
    Color renderPixel(int x, int y) const override;
    PathTracer(shared_ptr<World> world, shared_ptr<Camera> camera);
};


#endif //INC_2RAYTRACE_PATHTRACER_H
