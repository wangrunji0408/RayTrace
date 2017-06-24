//
// Created by 王润基 on 2017/4/19.
//

#include "GridTexture.h"

GridTexture::GridTexture(float n, const Color &a, const Color &b) :
        n(n), a(a), b(b) {}

Color GridTexture::operator()(Point const &uv) const {
    int x = (int)floorf(uv.x * n) & 1;
    int y = (int)floorf(uv.y * n) & 1;
    return x ^ y? a: b;
}
