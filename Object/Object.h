//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_OBJECT_H
#define INC_2RAYTRACE_OBJECT_H

#include <string>
#include "../Shapes/Shape.h"
#include "../Material/Material.h"
#include "../UVMaps/UVMap.h"
#include "../Material/ObjectMaterial.h"

using std::string;

class Object {
public:
    string name;
private:
    shared_ptr<Shape> shape;
    shared_ptr<ObjectMaterial> material;
    shared_ptr<UVMap> uvMap;
public:
    Object(shared_ptr<Shape> shape, shared_ptr<ObjectMaterial> material, shared_ptr<UVMap> uvMap = nullptr,
           string name = "object");
    shared_ptr<Shape> getShape() const;
    Material getMaterialAt(Vector3f const& point) const;
};


#endif //INC_2RAYTRACE_OBJECT_H
