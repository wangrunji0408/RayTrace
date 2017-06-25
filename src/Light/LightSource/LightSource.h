//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_LIGHTSOURCE_H
#define INC_2RAYTRACE_LIGHTSOURCE_H


#include "../Light.h"

class LightSource {
public:
    std::string name;
    Color color;
    bool shade = true;
    bool enable = true;
public:
    virtual Light illuminate (Vector3f const& point) const = 0;
    virtual std::vector<Light> illuminates (Vector3f const& point) const;
    virtual Ray sample() const = 0;
};


#endif //INC_2RAYTRACE_LIGHTSOURCE_H
