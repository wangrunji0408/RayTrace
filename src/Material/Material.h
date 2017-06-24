//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_MATERTIAL_H
#define INC_2RAYTRACE_MATERTIAL_H

#include "../Geometry/Geometry.h"
#include "../Texture/Texture.h"

class Material {
public:
    std::string name;
    Color emission = Color::zero;   // 自发光
    Color ambient = Color::zero;    // 环境光

    Color diffuse = Color::zero;    // 漫反射
    Color specular = Color::zero;   // 反射高光
    float shininess = 0;            // 反射光泽度

    Color tdiffuse = Color::zero;   // 漫透射
    Color tspecular = Color::zero;  // 透射高光
    float tshininess = 0;            // 透射光泽度

    Color attenuation = Color::zero;// 衰减
    float refractiveIndex = 1;      // 折射率
    float outRefractiveIndex = 1;   // 外部材质折射率（临时解决方案）

    Color reflection = Color::zero; // 反射系数
    Color refraction = Color::zero; // 折射系数（透明度）
protected:
    virtual Color calcCosBRDFUnit(Vector3f const &lUnit, Vector3f const &vUnit, Vector3f const &nUnit) const;
public:
    Color calcCosBRDF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) const;
    Color calcBRDF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) const;
    Color calcAttenuation (float dist) const;
    Color calcEmission (Vector3f const &outDir, Vector3f const &normalDir) const;
};


#endif //INC_2RAYTRACE_MATERTIAL_H
