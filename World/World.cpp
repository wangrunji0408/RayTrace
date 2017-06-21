//
// Created by 王润基 on 2017/4/8.
//

#include "World.h"
#include "../Shapes/2D/Shape2D.h"

IntersectInfo World::tryGetFirstIntersectionPoint(Ray const &ray) const {
    auto mintInfo = IntersectInfo(Ray());
    for(int i=0; i<objects.size(); ++i) {
        auto info = IntersectInfo(ray);
        info.needNormal = true;
        info.needUV = true;
        info.needParam = true;
        info.object = objects[i].get();
        objects[i]->intersect(info);
        if(info.success && info.t < mintInfo.t)
            mintInfo = info;
    }
    return mintInfo;
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

float World::calcG(Ray const &face1, Ray const &face2) {
    auto dp = face2.getStartPoint() - face1.getStartPoint();
    auto d2 = dp.len2();
    return fabsf(face1.getUnitDir().dot(dp) * face2.getUnitDir().dot(dp)) / d2 / d2;
}

bool World::testLightBlocked(Light const &light) const {
    return testLightBlocked(light.begin, light.end);
}

bool World::testLightBlocked(Point const &p1, Point const &p2) const {
    auto dp = p2 - p1;
    float d = dp.len();
    return testLightBlocked(Ray::raw(p1, dp / d), d);
}

bool World::testLightBlocked(const Ray &ray, float t) const {
    auto info = IntersectInfo(ray);
    info.testBlockT = t * 0.99f;
    for(int i=0; i<objects.size(); ++i) {
        objects[i]->intersect(info);
        if(info.success)
            return true;
    }
    return false;
}


