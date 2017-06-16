//
// Created by 王润基 on 2017/4/15.
//

#include "Object.h"

shared_ptr<Shape> Object::getShape() const {
    return shape;
}

Object::Object(shared_ptr<Shape> shape, shared_ptr<ObjectMaterial> material, shared_ptr<UVMap> uvMap, string name) :
        shape(shape), material(material), uvMap(uvMap), name(name) {}

Material Object::getMaterialAt(Vector3f const &uv) const {
    return material->getMaterial(uv);
}

void Object::applyTransform(Transform const &t) {
    transform = t * transform;
    transform_i = transform_i * t.inverse();
}

void Object::intersect(IntersectInfo &info) const {
    shape->intersect(info);
    if(!info.success || info.testBlockT != 0)    return;
    if(info.needUV && uvMap != nullptr)
        info.uv = uvMap->getUV(info.getPoint());
}
