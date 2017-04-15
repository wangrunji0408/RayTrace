//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_POINTLIGHT_H
#define INC_2RAYTRACE_POINTLIGHT_H


#include <ostream>
#include "LightSource.h"

class PointLight: public LightSource {
private:
    Vector3f pos;
    Color color;
public:
    const Vector3f &getPos() const;
    void setPos(const Vector3f &pos);
    const Color &getColor() const;
    void setColor(const Color &color);

    PointLight();
    PointLight(const Vector3f &pos, const Color &color);
    Light illuminate(Vector3f const &point) const override;

    friend std::ostream &operator<<(std::ostream &os, const PointLight &light);
};


#endif //INC_2RAYTRACE_POINTLIGHT_H
