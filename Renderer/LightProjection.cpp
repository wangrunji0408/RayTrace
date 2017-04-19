//
// Created by 王润基 on 2017/4/15.
//

#include "LightProjection.h"

Color LightProjection::renderPixel(int x, int y) const {
    auto ray = camera->getRay(x, y);
    auto result = world->tryGetFirstIntersectionPoint(ray);
    if(!result.isSuccess())
        return world->getEnvColor();
    auto obj = result.getObject();
    auto point = result.getPoint();
    auto material = obj->getMaterial();
    Color color = world->getEnvColor() * material->ambient;
    for(auto light : world->getLights())
    {
        Light l = light->illuminate(point);
        if(world->testLightBlocked(l))  continue;
        Color f = material->calcF(-l.getUnitDir(), ray.getStartPoint() - point, result.getNormal());
        color += l.color * f;
    }
    return color;
}

LightProjection::LightProjection(World *world, Camera *camera) : Renderer(world, camera) {}
