//
// Created by 王润基 on 2017/4/9.
//

#include <cmath>
#include "Material.h"

Color Material::calcCosBRDF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) const {
    auto i = inDir.norm();
    auto o = outDir.norm();
    auto n = normalDir.norm();
    return calcCosBRDFUnit(i, o, n);
}

Color Material::calcBRDF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) const {
    auto i = inDir.norm();
    auto o = outDir.norm();
    auto n = normalDir.norm();
    return calcCosBRDFUnit(i, o, n) / i.dot(n);
}

Color Material::calcAttenuation(float dist) const {
    auto t = attenuation * (-dist);
    return Color(expf(t.x), expf(t.y), expf(t.z));
}

Color Material::calcCosBRDFUnit(Vector3f const &l, Vector3f const &v, Vector3f const &n) const {
    float dotnl = n.dot(l), dotnv = n.dot(v);
    if(dotnl < -eps && dotnv < -eps)    // 背面反射
    {

//        Vector3f h = (l + v).norm();
//        float ks = powf(-n.dot(h), shininess);
//        return diffuse * -dotnl + specular * ks;
    }
    else if(dotnl > eps && dotnv > eps)  // 正面反射
    {
        Vector3f h = (l + v).norm();
        float ks = powf(n.dot(h), shininess);
        return diffuse * dotnl + specular * ks;
    }
    else if(dotnl < -eps && dotnv > eps) // 正面透射
    {
        float index = refractiveIndex / outRefractiveIndex;
        Vector3f h = (l * index + v).norm();
        if(index > 1)    h = -h;
        float ks = powf(n.dot(h), tshininess);
        return tdiffuse * (-dotnl) + tspecular * ks;
    }
    return Color::zero;
}

Color Material::calcEmission(Vector3f const &outDir, Vector3f const &normalDir) const {
    if(emission == 0)   return Color::zero;
    return emission * normalDir.norm().dot(outDir.norm());
}
