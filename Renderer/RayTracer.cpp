//
// Created by 王润基 on 2017/4/16.
//

#include "RayTracer.h"

Color RayTracer::renderPixel(int x, int y) const {
    return renderRay(camera->getRay(x, y), maxDepth, Color(1,1,1));
}

RayTracer::RayTracer(World *world, Camera *camera) : Renderer(world, camera) {}

Color RayTracer::renderRay(Ray const &ray, int depth, Color weight) const {
    if(depth == 0 || weight < epsColor)  return Color::zero;
    auto result = world->tryGetFirstIntersectionPoint(ray);
    if(!result.isSuccess())
        return world->getEnvColor();
    auto obj = result.getObject();
    auto point = result.getPoint();
    auto material = obj->getMaterial();
    auto color = Color::zero;
    for(auto light : world->getLights())
    {
        Light l = light->illuminate(point);
        if(world->testLightBlocked(l))  continue;
        Color f = material->calcF(-l.getUnitDir(), ray.getStartPoint() - point, result.getNormal());
        color += l.color * f;
    }
    {
        auto reflect = Ray::fromTo(point, result.getFace().calcSymmetryPoint(ray.getStartPoint()));
        Color f = material->calcF(reflect.getUnitDir(), ray.getStartPoint() - point, result.getNormal());
        if(!(f < epsColor))
            color += f * renderRay(reflect, depth - 1, weight * f);
    }
    if(!isZero(material->transparency))
    {
        auto refract = Ray(point, material->calcRefractiveDir(ray.getStartPoint() - point, result.getNormal()));
        Color f = obj->getMaterial()->calcF(refract.getUnitDir(), ray.getStartPoint() - point, result.getNormal());
        if(!(f < epsColor))
            color += f * renderRay(refract, depth - 1, weight * f);
    }
    return color;
}

int RayTracer::getMaxDepth() const {
    return maxDepth;
}

void RayTracer::setMaxDepth(int maxDepth) {
    RayTracer::maxDepth = maxDepth;
}
