//
// Created by 王润基 on 2017/4/8.
//

#include "IntersectResult.h"

IntersectResult IntersectResult::miss;

IntersectResult::IntersectResult(): success(false) {}

IntersectResult::IntersectResult(const Ray &ray, shared_ptr<Object> object, float t, Vector3f const &normal, Vector3f const &param)
        : ray(ray), face(Ray(ray.getEndPoint(t), normal)), t(t), object(object), param(param), success(true) {}

bool IntersectResult::isSuccess() const {
    return success;
}

bool IntersectResult::isOuter() const {
    return face.calcProjectionT(ray.getStartPoint()) > eps;
}

Vector3f IntersectResult::getPoint() const {
    return face.getStartPoint();
}

Ray IntersectResult::getFace() const {
    return face;
}

float IntersectResult::getT() const {
    return t;
}

shared_ptr<Object> IntersectResult::getObject() const {
    return object;
}

Vector3f IntersectResult::getNormal() const {
    return face.getUnitDir();
}

const Vector3f &IntersectResult::getParam() const {
    return param;
}

std::ostream &operator<<(std::ostream &os, const IntersectResult &result) {
    os << "[IntersectResult success: " << result.success;
    if(result.success)
        os << " ray: " << result.ray << " face: " << result.face
           << " t: " << result.t << " object: " << result.object->name;
    return os << "]";
}

