//
// Created by 王润基 on 2017/4/16.
//

#ifndef INC_2RAYTRACE_RAYTRACER_H
#define INC_2RAYTRACE_RAYTRACER_H


#include "Renderer.h"

class RayTracer: public Renderer {
protected:
    int maxDepth = 2;
    Color epsColor = Color(1e-3f, 1e-3f, 1e-3f);
protected:
    Color renderRay (Ray const& ray, int depth, Color weight) const;
public:
    RayTracer(World *world, Camera *camera);
    Color renderPixel(int x, int y) const override;
    int getMaxDepth() const;
    void setMaxDepth(int maxDepth);
};


#endif //INC_2RAYTRACE_RAYTRACER_H
