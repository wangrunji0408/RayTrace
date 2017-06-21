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

PathTracer::PathTracer(shared_ptr<World> world, shared_ptr<Camera> camera) : RayTracer(world, camera) {}

Color PathTracer::renderRay(Ray const &ray0, int depth, Color weight) const {
    Ray ray = ray0;
    Color color = Color::zero;
//    Ray lastFace = ray0;
    while(depth-- && !(weight < epsColor))
    {
        auto result = world->tryGetFirstIntersectionPoint(ray);
        if(!result.success) {
            color += weight * world->getEnvColor();
            break;
        }
        auto obj = result.object;
        auto point = result.getPoint();
        auto material = obj->getMaterialAt(result.uv);
        auto v = ray.getStartPoint() - point;
        auto n = result.normal;
//        auto face = Ray(point, n);
//        weight *= World::calcG(lastFace, face);
        color += weight * material.calcEmission(v, n);
        for(auto const& light : world->getLights())
        {
            Light l = light->illuminate(point);
            if(world->testLightBlocked(l))  continue;
            if(saveLights) lights->push_back(l);
            auto brdf = material.calcCosBRDF(-l.getUnitDir(), v, n);
            color += weight * brdf * l.color;
        }
        Color brdf; Vector3f l;
        if(!(material.reflection < epsColor))
        {
            l = World::calcReflectiveDir(v, n);
            brdf = material.reflection;
        }
        else if(!(material.refraction < epsColor))
        {
            l = World::calcRefractiveDir(v, n, material.refractiveIndex, material.outRefractiveIndex);
            brdf = material.refraction;
        }
        else
        {
            l = Vector3f::getRandUnit();
            if(material.tdiffuse < epsColor && material.tspecular < epsColor) {
                v = v.forcePositiveBy(n);
                weight *= 0.5;
            }
            brdf = material.calcCosBRDF(l, v, n);
        }
        ray = Ray(point + l * 1e-4, l);
        weight *= brdf;
//        lastFace = face;
    }
    return color;
}
