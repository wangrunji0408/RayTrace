//
// Created by 王润基 on 2017/4/19.
//

#include "PathTracer.h"

Color PathTracer::renderPixel(int x, int y) const {
    auto ray = camera->getRay(x, y);
    auto color = Color::zero;
    for(int i=0; i<times; ++i)
        color += renderRay(ray, maxDepth, Color(1,1,1));
    return color / times;
}

PathTracer::PathTracer(World *world, Camera *camera) : RayTracer(world, camera) {}

Color PathTracer::renderRay(Ray const &ray, int depth, Color weight) const {
    if(depth == 0 || weight < epsColor)  return Color::zero;
    auto result = world->tryGetFirstIntersectionPoint(ray);
    if(!result.isSuccess())
        return world->getEnvColor();
    auto obj = result.getObject();
    auto point = result.getPoint();
    auto material = obj->getMaterial();
    auto v = ray.getStartPoint() - point;
    auto n = result.getNormal();
    Color color = material->calcEmission(v, n);
    for(auto const& light : world->getLights())
    {
        Light l = light->illuminate(point);
        if(world->testLightBlocked(l))  continue;
        if(saveLights) lights->push_back(l);
        Color f = material->calcF(-l.getUnitDir(), v, n);
        color += l.color * f;
    }
    {
//        auto reflect = Ray(point, World::calcReflectiveDir(v, n));
//        reflect = Ray(reflect.getEndPoint(1e-4f), reflect.getUnitDir());    // 防止自相交
//        auto refract = Ray(point, World::calcRefractiveDir(v, n,
//                                                           material->refractiveIndex,
//                                                           material->outRefractiveIndex));
//        refract = Ray(refract.getEndPoint(1e-4f), refract.getUnitDir());
        auto l = Vector3f::getRandUnit();
        auto ray = Ray(point + l * 1e-4f, l);
        Color f = material->calcF(l, v, n);
        color += f * renderRay(ray, depth - 1, weight * f);
    }
    if(saveLights)
        lights->push_back(Light(point, ray.getStartPoint(), color));
    return color;
}
