//
// Created by 王润基 on 2017/4/15.
//

#include "Object.h"

shared_ptr<Shape> Object::getShape() const {
    return shape;
}

Object::Object(shared_ptr<Shape> shape, shared_ptr<ObjectMaterial> material, shared_ptr<UVMap> uvMap, string name) :
        shape(shape), material(material), uvMap(uvMap), name(name) {}

Material Object::getMaterialAt(Vector3f const &point) const {
    Point uv = uvMap == nullptr? shape->getUV(point): uvMap->getUV(point);
    return material->getMaterial(uv);
}

void Object::applyTransform(Transform const &t) {
    transform = t * transform;
    transform_i = t.inverse() * transform_i;
}
