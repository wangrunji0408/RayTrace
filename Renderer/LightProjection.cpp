//
// Created by 王润基 on 2017/4/15.
//

#include "LightProjection.h"

LightProjection::LightProjection(shared_ptr<World> world, shared_ptr<Camera> camera) : Renderer(world, camera) {}

Color LightProjection::renderRay(Ray const &ray) const {
    auto result = world->tryGetFirstIntersectionPoint(ray);
    if(!result.isSuccess())
        return world->getEnvColor();
    auto obj = result.getObject();
    auto point = result.getPoint();
    auto material = obj->getMaterialAt(point);
    Color color = world->getEnvColor() * material.ambient;
    for(auto light : world->getLights())
    {
        Light l = light->illuminate(point);
        if(world->testLightBlocked(l))  continue;
        Color f = material.calcF(-l.getUnitDir(), ray.getStartPoint() - point, result.getNormal());
        color += l.color * f;
    }
    return color;
}
