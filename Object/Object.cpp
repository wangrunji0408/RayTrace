//
// Created by 王润基 on 2017/4/15.
//

#include "Object.h"

Shape* Object::getShape() const {
    return shape;
}

Object::Object(Shape *shape, Material *material, string name) :
        shape(shape), material(material), name(name) {}

Material *Object::getMaterial() const {
    return material;
}

const std::string &Object::getName() const {
    return name;
}
