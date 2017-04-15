//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_WORLD_H
#define INC_2RAYTRACE_WORLD_H

#include <vector>
#include "../Light/Light.h"
#include "../Object/Object.h"
#include "../Light/LightSource/LightSource.h"
#include "../Object/IntersectResult.h"

class World {
    std::vector<Object*> objects;
    std::vector<LightSource*> lights;
public:
    const std::vector<LightSource *> &getLights() const;
    void addObject (Object* obj);
    void removeObject (Object* obj);
    void addLight (LightSource* light);
    void removeLight (LightSource* light);
    IntersectResult tryGetFirstIntersectionPoint (Ray const& ray) const;
};


#endif //INC_2RAYTRACE_WORLD_H
