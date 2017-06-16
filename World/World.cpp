//
// Created by 王润基 on 2017/4/8.
//

#include "World.h"
#include "../Shapes/2D/Shape2D.h"

IntersectResult World::tryGetFirstIntersectionPoint(Ray const &ray, float tmin) const {
    std::vector<float> ts(objects.size(), inf);
    std::vector<Vector3f> normals(objects.size());
    std::vector<Vector3f> params(objects.size());
    int index = -1;
    for(int i=0; i<objects.size(); ++i) {
        auto shape = std::static_pointer_cast<Shape2D>(objects[i]->getShape());
        if(shape == nullptr)
            continue;
        bool exist = shape->tryGetIntersectionInfo(ray, ts[i], params[i], normals[i]);
        if(!exist)
            ts[i] = inf;
        else if(ts[i] < tmin) {
            index = i;
            break;
        }
    }
    if(index == -1)
        index = int(std::min_element(ts.begin(), ts.end()) - ts.begin());
    if(ts[index] == inf)
        return IntersectResult::miss;
    return IntersectResult(ray, objects[index], ts[index], normals[index], params[index]);
}

void World::addObject(shared_ptr<Object> obj) {
    objects.push_back(obj);
}

void World::removeObject(shared_ptr<Object> obj) {
    objects.erase(std::find(objects.begin(), objects.end(), obj));
}

void World::addLight(shared_ptr<LightSource> light) {
    lights.push_back(light);
    if(light->enable)
        enabledLights.push_back(light);
}

void World::removeLight(shared_ptr<LightSource> light) {
    lights.erase(std::find(lights.begin(), lights.end(), light));
    if(light->enable)
        enabledLights.erase(std::find(enabledLights.begin(), enabledLights.end(), light));
}

const std::vector<shared_ptr<LightSource>> &World::getLights() const {
    return enabledLights;
}

void World::addCamera(shared_ptr<Camera> camera) {
    cameras.push_back(camera);
}

void World::removeCamera(shared_ptr<Camera> camera) {
    cameras.erase(std::find(cameras.begin(), cameras.end(), camera));
}

shared_ptr<Camera> World::findCamera(std::string name) const {
    auto it = std::find_if(cameras.begin(), cameras.end(),
                        [&](shared_ptr<Camera> const& camera){return camera->name == name;});
    return it == cameras.end()? nullptr: *it;
}

bool World::testLightBlocked(Light const &light) const {
    Ray const& ray = light.getRay();
    float tmin = light.len() * 0.99f;
    for(int i=0; i<objects.size(); ++i) {
        auto shape = std::static_pointer_cast<Shape2D>(objects[i]->getShape());
        if(shape == nullptr)
            continue;
        if(shape->testRayBlocked(ray, tmin))
            return true;
    }
    return false;
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

shared_ptr<Object> World::findObject(std::string name) const {
    auto it = std::find_if(objects.begin(), objects.end(),
                           [&](shared_ptr<Object> const& object){return object->name == name;});
    return it == objects.end()? nullptr: *it;
}


