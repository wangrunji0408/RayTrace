//
// Created by 王润基 on 2017/4/8.
//

#include "IntersectInfo.h"
#include "Geometry.h"
#include "../Object/Object.h"
#include "../Material/Material.h"

using std::endl;

bool IntersectInfo::isOuter() const {
    if(!needNormal)
        throw std::invalid_argument("Try to test isOuter, but needNormal is false.");
    return Ray(getPoint(), normal).calcProjectionT(ray.getStartPoint()) > eps;
}

Vector3f IntersectInfo::getPoint() const {
    return ray.getEndPoint(t);
}

IntersectInfo::IntersectInfo(const Ray &ray) : ray(ray) {}

Material IntersectInfo::getMaterial() const {
    return object->getMaterialAt(uv);
}

std::ostream &operator<<(std::ostream &os, const IntersectInfo &info) {
    os << "[IntersectInfo ray: " << info.ray << " testBlockT: " << info.testBlockT << endl;
    if(info.success) {
        os << "success" << endl;
        os << "\tt: " << info.t << endl;
        os << "\tpoint: " << info.getPoint() << endl;
        if(info.needNormal)
            os << "\tnormal: " << info.normal << endl;
        if(info.needParam)
            os << "\tuv: " << info.param << endl;
        if(info.needUV)
            os << "\tparam: " << info.uv << endl;
        if(info.needObject)
            os << "\tobject: " << info.object->name << endl;
    }
    else
        os << "\tfailed" << endl;
    return os << "]";
}

