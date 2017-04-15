//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_LAMBERTAIN_H
#define INC_2RAYTRACE_LAMBERTAIN_H


#include "Material.h"

class Lambertain: public Material {
public:
    Color diffuse;
public:
    Lambertain(Color color);

protected:
    Color calcBRDF(Vector3f const &l, Vector3f const &v, Vector3f const &n) const override;
};


#endif //INC_2RAYTRACE_LAMBERTAIN_H
