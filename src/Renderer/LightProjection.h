//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_LIGHTPROJECTION_H
#define INC_2RAYTRACE_LIGHTPROJECTION_H


#include "Renderer.h"

class LightProjection: public Renderer {
public:
    LightProjection(shared_ptr<World> world, shared_ptr<Camera> camera);

    virtual Color renderRay(Ray const &ray) const;
};


#endif //INC_2RAYTRACE_LIGHTPROJECTION_H
