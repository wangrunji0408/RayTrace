//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_WORLD_H
#define INC_2RAYTRACE_WORLD_H

#include <vector>
#include "../Shapes/Shape.h"
#include "../Light/Light.h"

class World {
    std::vector<Shape*> objects;
public:
    void addObject (Shape* obj);
    void removeObject (Shape* obj);
    bool tryGetFirstIntersectionPoint (Ray const& ray, Shape* &obj, float& t) const;
    Color rayTrace (Ray const& ray) const;
};


#endif //INC_2RAYTRACE_WORLD_H
