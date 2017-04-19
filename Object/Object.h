//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_OBJECT_H
#define INC_2RAYTRACE_OBJECT_H

#include <string>
#include "../Shapes/Shape.h"
#include "../Material/Material.h"
#include "../Texture/UVMaps/UVMap.h"
#include "../Material/ObjectMaterial.h"

using std::string;

class Object {
public:
    string name;
private:
    Shape* shape;
    ObjectMaterial* material;
    UVMap* uvMap = nullptr;
public:
    Object(Shape *shape, ObjectMaterial *material, UVMap *uvMap = nullptr, string name = "object");
    const string &getName() const;
    Shape* getShape() const;
    Material getMaterialAt(Vector3f const& point) const;
};


#endif //INC_2RAYTRACE_OBJECT_H
