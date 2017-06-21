//
// Created by 王润基 on 2017/4/19.
//

#ifndef INC_2RAYTRACE_OBJECTMATERIAL_H
#define INC_2RAYTRACE_OBJECTMATERIAL_H


#include "Material.h"

class ObjectMaterial {
public:
    std::string name;
    shared_ptr<Texture> diffuse = nullptr;    // 漫反射
    shared_ptr<Texture> ambient = nullptr;    // 环境光
    Material m;
public:
    Material getMaterial (Vector3f const& uv) const;
};


#endif //INC_2RAYTRACE_OBJECTMATERIAL_H
