//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_OBJECT_H
#define INC_2RAYTRACE_OBJECT_H

#include <string>
#include "../Shapes/Shape.h"
#include "../Material/Material.h"
#include "../Texture/UVMap.h"

using std::string;

class Object {
private:
    string name;
    Shape* shape;
    Material* material;
    UVMap* uvMap;
public:
    Object(Shape *shape, Material *material, string name = "object");
    const string &getName() const;
    Shape* getShape() const;
    Material *getMaterial() const;
};


#endif //INC_2RAYTRACE_OBJECT_H
