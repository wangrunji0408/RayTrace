//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_MODIFIEDPHONG_H
#define INC_2RAYTRACE_MODIFIEDPHONG_H


#include "Phong.h"

class ModifiedPhong: public Phong {
public:
    ModifiedPhong(const Color &diffuse, const Color &specular, float shininess);
    Color getF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) override;
};


#endif //INC_2RAYTRACE_MODIFIEDPHONG_H
