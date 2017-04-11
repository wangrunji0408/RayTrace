//
// Created by 王润基 on 2017/4/8.
//

#include "World.h"
#include <algorithm>

bool World::tryGetFirstIntersectionPoint(Ray const &ray, Shape *&obj, float &t) const {
    std::vector<float> ts(objects.size(), inf);
    for(int i=0; i<objects.size(); ++i) {
        auto obj = static_cast<IRayCastable*>(objects[i]);
        if(obj == nullptr)
            continue;
        bool exist = obj->tryGetIntersectionPoint(ray, ts[i]);
//        if(!exist)
//            ts[i] = inf;
    }
    auto it = std::min_element(ts.begin(), ts.end());
    if(*it == inf)
        return false;
    obj = objects[it - ts.begin()];
    t = *it;
    return true;
}

void World::addObject(Shape *obj) {
    objects.push_back(obj);
}

void World::removeObject(Shape *obj) {
    objects.erase(std::find(objects.begin(), objects.end(), obj));
}
