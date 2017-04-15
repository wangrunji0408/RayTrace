//
// Created by 王润基 on 2017/4/9.
//

#include "Material.h"

Color Material::getFByPoint(Vector3f const &inPoint, Vector3f const &outPoint, Ray const &face) {
    Vector3f const& o = face.getStartPoint();
    Vector3f const& n = face.getUnitDir();
    Vector3f l = inPoint - o;
    Vector3f r = outPoint - o;
    return getF(l, r, n);
}
