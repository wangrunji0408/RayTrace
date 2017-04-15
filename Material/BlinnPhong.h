//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_BLINNPHONG_H
#define INC_2RAYTRACE_BLINNPHONG_H


#include "Phong.h"

class BlinnPhong: public Phong {
public:
    BlinnPhong(const Color &diffuse, const Color &specular, float shininess);

protected:
    Color calcBRDF(Vector3f const &l, Vector3f const &v, Vector3f const &n) const override;
};


#endif //INC_2RAYTRACE_BLINNPHONG_H
