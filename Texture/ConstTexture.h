//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_CONSTTEXTURE_H
#define INC_2RAYTRACE_CONSTTEXTURE_H


#include "Texture.h"

class ConstTexture: public Texture {
public:
    Color color;
    ConstTexture(const Color &color);
    Color operator()(Point const &uv) const override;
};


#endif //INC_2RAYTRACE_CONSTTEXTURE_H
