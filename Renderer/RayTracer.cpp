//
// Created by 王润基 on 2017/4/16.
//

#include "RayTracer.h"

RayTracer::RayTracer(shared_ptr<World> world, shared_ptr<Camera> camera) : Renderer(world, camera) {}

Color RayTracer::renderRay(Ray const &ray, int depth, Color weight) const {
    if(depth == 0 || weight < epsColor)  return Color::zero;
    auto result = world->tryGetFirstIntersectionPoint(ray);
    if(!result.isSuccess())
        return world->getEnvColor();
    auto obj = result.getObject();
    auto point = result.getPoint();
    auto material = obj->getMaterialAt(point);
    auto v = ray.getStartPoint() - point;
    auto n = result.getNormal();
    Color color = Color::zero;
    color += world->getEnvColor() * material.ambient;
    color += material.calcEmission(v, n);
    for(auto const& light : world->getLights())
    {
        Light l = light->illuminate(point);
        if(light->shade && world->testLightBlocked(l))  continue;
        if(saveLights) lights->push_back(l);
        Color f = material.calcF(-l.getUnitDir(), v, n);
        color += l.color * f;
    }
    if(!(material.reflection < epsColor))
    {
        auto reflect = Ray(point, World::calcReflectiveDir(v, n));
        reflect = Ray(reflect.getEndPoint(1e-4f), reflect.getUnitDir());    // 防止自相交
        Color f = material.reflection;
        color += f * renderRay(reflect, depth - 1, weight * f);
    }
    if(!(material.refraction < epsColor))
    {
        auto refract = Ray(point, World::calcRefractiveDir(v, n,
                                                           material.refractiveIndex,
                                                           material.outRefractiveIndex));
        refract = Ray(refract.getEndPoint(1e-4f), refract.getUnitDir());
        Color f = material.refraction;
        color += f * renderRay(refract, depth - 1, weight * f);
    }
    if(saveLights)
        lights->push_back(Light(point, ray.getStartPoint(), color));
    return color;
}

int RayTracer::getMaxDepth() const {
    return maxDepth;
}

void RayTracer::setMaxDepth(int maxDepth) {
    RayTracer::maxDepth = maxDepth;
}

std::vector<Light> RayTracer::renderPixelGetLights(int x, int y) {
    saveLights = true;
    renderPixel(x, y);
    saveLights = false;
    auto ans = *lights;
    lights->clear();
    return ans;
}

Color RayTracer::renderRay(Ray const &ray) const {
    return renderRay(ray, maxDepth, Color(1,1,1));
}
