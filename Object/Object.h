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
#include "../Geometry/Transform.h"

using std::string;

class Object {
public:
    string name;
private:
    Transform transform, transform_i;   // Shape坐标系 相对 World坐标系 的变换矩阵，及逆
    shared_ptr<Shape> shape;
    shared_ptr<ObjectMaterial> material;
    shared_ptr<UVMap> uvMap;
public:
    Object(shared_ptr<Shape> shape, shared_ptr<ObjectMaterial> material, shared_ptr<UVMap> uvMap = nullptr,
           string name = "object");
    shared_ptr<Shape> getShape() const;
    Material getMaterialAt(Vector3f const& point) const;
    void applyTransform (Transform const& t);
};


#endif //INC_2RAYTRACE_OBJECT_H
