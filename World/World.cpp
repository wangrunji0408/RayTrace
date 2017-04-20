//
// Created by 王润基 on 2017/4/8.
//

#include "World.h"
#include "../Shapes/2D/Shape2D.h"

IntersectResult World::tryGetFirstIntersectionPoint(Ray const &ray) const {
    std::vector<float> ts(objects.size(), inf);
    std::vector<Vector3f> normals(objects.size());
    for(int i=0; i<objects.size(); ++i) {
        auto shape = static_cast<Shape2D*>(objects[i]->getShape());
        if(shape == nullptr)
            continue;
        Point point;
        bool exist = shape->tryGetIntersectionInfo(ray, ts[i], point, normals[i]);
        if(!exist)
            ts[i] = inf;
    }
    auto it = std::min_element(ts.begin(), ts.end());
    if(*it == inf)
        return IntersectResult::miss;

    int index = (int)(it - ts.begin());
    auto object = objects[index];
    float t = *it;
    auto point = ray.getEndPoint(t);
    auto shape2d = dynamic_cast<Shape2D*>(object->getShape());
    auto normal = normals[index];
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
    if(light->enable)
        enabledLights.push_back(light);
}

void World::removeLight(LightSource *light) {
    lights.erase(std::find(lights.begin(), lights.end(), light));
    if(light->enable)
        enabledLights.erase(std::find(enabledLights.begin(), enabledLights.end(), light));
}

const std::vector<LightSource *, std::allocator<LightSource *>> &World::getLights() const {
    return enabledLights;
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

const Color &World::getEnvColor() const {
    return envColor;
}

void World::setEnvColor(const Color &envColor) {
    World::envColor = envColor;
}

Vector3f World::calcRefractiveDir(Vector3f const &inDir, Vector3f const &normalDir, float indexInside, float indexOutside) {
    auto l = inDir.norm();
    auto n = normalDir.norm();
    auto m = n.det(l);
    float index = indexInside / indexOutside;
    float sini = m.len();
    if(n.dot(l) > eps) {
        float sinr = sini / index;
        float cosr = sqrtf(1 - sinr * sinr);
        auto rr = n.det(m).norm();
        return n * (-cosr) + rr * sinr;
    }
    else
    {
        float sinr = sini * index;
        float cosr = sqrtf(1 - sinr * sinr);
        auto rr = n.det(m).norm();
        return n * cosr + rr * sinr;
    }
}

Vector3f World::calcReflectiveDir(Vector3f const &inDir, Vector3f const &normalDir) {
    auto l = inDir.norm();
    auto n = normalDir.norm();
    return n * 2 * n.dot(l) - l;
}


