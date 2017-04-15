//
// Created by 王润基 on 2017/4/9.
//

#include "Material.h"

Color Material::calcFByPoint(Vector3f const &inPoint, Vector3f const &outPoint, Ray const &face) const {
    Vector3f const& o = face.getStartPoint();
    Vector3f const& n = face.getUnitDir();
    Vector3f l = inPoint - o;
    Vector3f r = outPoint - o;
    return calcF(l, r, n);
}

Color Material::calcF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) const {
    return calcBRDF(inDir.norm(), outDir.norm(), normalDir.norm());
}
