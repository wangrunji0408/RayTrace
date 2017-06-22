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
    return getObject()->getMaterialAt(uv);
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
        auto shape = info.getShape();
        if(shape != nullptr)
            os << "\tshape: " << shape->name << endl;
        auto obj = info.getObject();
        if(obj != nullptr)
            os << "\tobject: " << obj->name << endl;
    }
    else
        os << "\tfailed" << endl;
    return os << "]";
}

Object *IntersectInfo::getObject() const {
    auto obj = dynamic_cast<const Object*>(object);
    if(obj == nullptr)
        throw std::runtime_error("Try to getObject, but IRayCastable is not Object.");
    return (Object *)obj;
}

Shape *IntersectInfo::getShape() const {
    auto obj = dynamic_cast<const Shape*>(object);
    if(obj == nullptr)
        throw std::runtime_error("Try to getShape, but IRayCastable is not Shape.");
    return (Shape *)obj;
}

