//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_TEXTURE_H
#define INC_2RAYTRACE_TEXTURE_H

#include <cmath>
#include "../Light/Light.h"

class Texture {
public:
    std::string name;
    virtual Color operator() (Point const& uv) const = 0;
};

inline float to01 (float x)
{
    return x - floorf(x);
}

#endif //INC_2RAYTRACE_TEXTURE_H
