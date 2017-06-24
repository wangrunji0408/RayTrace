//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_GRIDTEXTURE_H
#define INC_2RAYTRACE_GRIDTEXTURE_H


#include "Texture.h"

class GridTexture: public Texture {
    float n;
    Color a, b;
public:
    GridTexture(float n = 8, const Color &a = Color::zero, const Color &b = Color(1,1,1));
    Color operator()(Point const &uv) const override;
};


#endif //INC_2RAYTRACE_GRIDTEXTURE_H
