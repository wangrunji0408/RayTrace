//
// Created by 王润基 on 2017/4/9.
//

#include <cmath>
#include "Material.h"

Color Material::calcFByPoint(Vector3f const &inPoint, Vector3f const &outPoint, Ray const &face) const {
    Vector3f const& o = face.getStartPoint();
    Vector3f const& n = face.getUnitDir();
    Vector3f l = inPoint - o;
    Vector3f r = outPoint - o;
    return calcF(l, r, n);
}

Color Material::calcF(Vector3f const &inDir, Vector3f const &outDir, Vector3f const &normalDir) const {
    auto i = inDir.norm();
    auto o = outDir.norm();
    auto n = normalDir.norm();
    auto dot = n.dot(i) * n.dot(o);
    if(dot < 0) return Color(transparency * (-dot));
    return calcBRDF(i, o, n);
}

Vector3f Material::calcRefractiveDir(Vector3f const &inDir, Vector3f const &normalDir) const {
    auto l = inDir.norm();
    auto n = normalDir.norm();
    auto m = l.det(n);
    float sini = m.len();
    float sinr = sini / refractiveIndex;
    float cosr = sqrtf(1 - sinr * sinr);
    auto rr = n.det(m).norm();
    return n * (-cosr) + rr * sinr;
}
