//
// Created by 王润基 on 2017/4/15.
//

#include "LightProjection.h"

LightProjection::LightProjection(shared_ptr<World> world, shared_ptr<Camera> camera) : Renderer(world, camera) {}

Color LightProjection::renderRay(Ray const &ray) const {
    auto result = world->tryGetFirstIntersectionPoint(ray);
    if(!result.success)
        return world->getEnvColor();

    auto point = result.getPoint();
    auto material = result.getMaterial();
    Color color = world->getEnvColor() * material.ambient;
    for(auto light : world->getLights())
    {
        Light l = light->illuminate(point);
        if(world->testLightBlocked(l))  continue;
        Color f = material.calcCosBRDF(-l.getUnitDir(), ray.getStartPoint() - point, result.normal);
        color += l.color * f / l.len2();
    }
    return color;
}
