//
// Created by 王润基 on 2017/4/8.
//

#include "World.h"
#include "../Shapes/2D/Shape2D.h"

IntersectResult World::tryGetFirstIntersectionPoint(Ray const &ray) const {
    std::vector<float> ts(objects.size(), inf);
    for(int i=0; i<objects.size(); ++i) {
        auto shape = static_cast<IRayCastable*>(objects[i]->getShape());
        if(shape == nullptr)
            continue;
        bool exist = shape->tryGetIntersectionPoint(ray, ts[i]);
        if(!exist)
            ts[i] = inf;
    }
    auto it = std::min_element(ts.begin(), ts.end());
    if(*it == inf)
        return IntersectResult::miss;

    auto object = objects[it - ts.begin()];
    float t = *it;
    auto point = ray.getEndPoint(t);
    auto shape2d = dynamic_cast<Shape2D*>(object->getShape());
    auto normal = Vector3f::zero;
    if(shape2d != nullptr)
        normal = shape2d->getNormalVectorOnSurface(point);
    return IntersectResult(ray, object, t, normal);
}

void World::addObject(Object *obj) {
    objects.push_back(obj);
}

void World::removeObject(Object *obj) {
    objects.erase(std::find(objects.begin(), objects.end(), obj));
}

void World::addLight(LightSource *light) {
    lights.push_back(light);
}

void World::removeLight(LightSource *light) {
    lights.erase(std::find(lights.begin(), lights.end(), light));
}

const std::vector<LightSource *, std::allocator<LightSource *>> &World::getLights() const {
    return lights;
}

void World::addCamera(Camera *camera) {
    cameras.push_back(camera);
}

void World::removeCamera(Camera *camera) {
    cameras.erase(std::find(cameras.begin(), cameras.end(), camera));
}

Camera *World::findCamera(std::string name) const {
    auto it = std::find_if(cameras.begin(), cameras.end(),
                        [&](Camera* const& camera){return camera->name == name;});
    return it == cameras.end()? nullptr: *it;
}

bool World::testLightBlocked(Light const &light) const {
    auto result = tryGetFirstIntersectionPoint(light.getRay());
    return result.isSuccess() && result.getT() < light.len() * 0.99f;
}


