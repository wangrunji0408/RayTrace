//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_LIGHTPROJECTION_H
#define INC_2RAYTRACE_LIGHTPROJECTION_H


#include "Renderer.h"

class LightProjection: public Renderer {
public:
    LightProjection(World *world, Camera *camera);
    Color renderPixel(int x, int y) const override;
};


#endif //INC_2RAYTRACE_LIGHTPROJECTION_H
