//
// Created by 王润基 on 2017/4/16.
//

#ifndef INC_2RAYTRACE_RAYTRACER_H
#define INC_2RAYTRACE_RAYTRACER_H

#include "Renderer.h"
#include <vector>

class RayTracer: public Renderer {
protected:
    bool saveLights = false;
    std::vector<Light>* lights = new std::vector<Light>();
protected:
    int maxDepth = 2;
    Color epsColor = Color(1e-3f, 1e-3f, 1e-3f);
protected:
    virtual Color renderRay (Ray const& ray, int depth, Color weight) const;
public:
    RayTracer(World *world, Camera *camera);
    Color renderPixel(int x, int y) const override;
    std::vector<Light> renderPixelGetLights(int x, int y);
    int getMaxDepth() const;
    void setMaxDepth(int maxDepth);
};


#endif //INC_2RAYTRACE_RAYTRACER_H
