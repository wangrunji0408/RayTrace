//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_MATERTIAL_H
#define INC_2RAYTRACE_MATERTIAL_H

#include "../Light/Light.h"
#include "../Geometry/Geometry.h"

class Material {
public:
    virtual Color getF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) = 0;
    virtual Color getFByPoint (Vector3f const& inPoint, Vector3f const& outPoint, Ray const& face);
};


#endif //INC_2RAYTRACE_MATERTIAL_H
