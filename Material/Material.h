//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_MATERTIAL_H
#define INC_2RAYTRACE_MATERTIAL_H

#include "../Light/Light.h"
#include "../Geometry/Geometry.h"

class Material {
public:
    std::string name;
    float refractiveIndex = 1;
    float transparency = 0;
protected:
    virtual Color calcBRDF(Vector3f const &lUnit, Vector3f const &vUnit, Vector3f const &nUnit) const = 0;
public:
    Color calcF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) const;
    Color calcFByPoint(Vector3f const &inPoint, Vector3f const &outPoint, Ray const &face) const;
    Vector3f calcRefractiveDir (Vector3f const &inDir, Vector3f const &normalDir) const;
};


#endif //INC_2RAYTRACE_MATERTIAL_H
