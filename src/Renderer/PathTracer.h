//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_PATHTRACER_H
#define INC_2RAYTRACE_PATHTRACER_H


#include "Renderer.h"
#include "RayTracer.h"

class PathTracer: public RayTracer {
protected:
    Vector3f randChoice(Material const &material, Vector3f const &n, Vector3f const &v,
                    Vector3f &l, bool reverse) const;
public:
    PathTracer(shared_ptr<World> world, shared_ptr<Camera> camera);
    Color renderRay(Ray const &ray) const override;
};


#endif //INC_2RAYTRACE_PATHTRACER_H
