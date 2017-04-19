//
// Created by 王润基 on 2017/4/15.
//

#include "Object.h"

Shape* Object::getShape() const {
    return shape;
}

Object::Object(Shape *shape, ObjectMaterial *material, UVMap *uvMap, string name) :
        shape(shape), material(material), uvMap(uvMap), name(name) {}

const std::string &Object::getName() const {
    return name;
}

Material Object::getMaterialAt(Vector3f const &point) const {
    Point uv = uvMap == nullptr? shape->getUV(point): uvMap->getUV(point);
    return material->getMaterial(uv);
}
