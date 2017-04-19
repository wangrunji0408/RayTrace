//
// Created by 王润基 on 2017/4/19.
//

#include "ConstTexture.h"

Color ConstTexture::operator()(Point const &uv) const {
    return color;
}

ConstTexture::ConstTexture(const Color &color) : color(color) {}
