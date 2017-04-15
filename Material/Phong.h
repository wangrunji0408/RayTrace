//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_PHONG_H
#define INC_2RAYTRACE_PHONG_H


#include "Material.h"

class Phong: public Material {
public:
    Color diffuse, specular;
    float shininess;
protected:
    Color calcBRDF(Vector3f const &l, Vector3f const &v, Vector3f const &n) const override;
public:
    Phong(const Color &diffuse, const Color &specular, float shininess);
};


#endif //INC_2RAYTRACE_PHONG_H
