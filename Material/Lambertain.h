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
    Color getF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) override;
};


#endif //INC_2RAYTRACE_LAMBERTAIN_H
