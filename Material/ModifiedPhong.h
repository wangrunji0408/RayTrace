//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_MODIFIEDPHONG_H
#define INC_2RAYTRACE_MODIFIEDPHONG_H


#include "Phong.h"

class ModifiedPhong: public Phong {
public:
protected:
    Color calcBRDF(Vector3f const &l, Vector3f const &v, Vector3f const &n) const override;
public:
    ModifiedPhong(const Color &diffuse, const Color &specular, float shininess);
};


#endif //INC_2RAYTRACE_MODIFIEDPHONG_H
