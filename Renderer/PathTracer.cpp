//
// Created by 王润基 on 2017/4/19.
//

#include "PathTracer.h"

PathTracer::PathTracer(shared_ptr<World> world, shared_ptr<Camera> camera) : RayTracer(world, camera) {}

Color PathTracer::renderRay(Ray const &ray0) const {
    Color color = Color::zero;
    Color weight = Color::zero;
    Ray ray = ray0;
    for(int depth = 0; depth < maxDepth && !(weight == 0); depth++)
    {
        auto result = world->tryGetFirstIntersectionPoint(ray);
        if(!result.success) {
            color += weight * world->getEnvColor();
            break;
        }
        auto point = result.getPoint();
        auto material = result.getMaterial();
        auto v = ray.getStartPoint() - point;
        auto n = result.normal;

        color += weight * material.calcEmission(v, n);
        for(auto const& light : world->getLights())
        {
            Light l = light->illuminate(point);
            if(world->testLightBlocked(l))  continue;
            if(saveLights) lights->push_back(l);
            auto brdf = material.calcCosBRDF(-l.getUnitDir(), v, n);
            color += weight * brdf * l.color / l.len2();
        }
        Color brdf; Vector3f l; int times;
        weight *= randChoice(material, n, v, l, true);
        ray = Ray(point + l * 1e-4, l);
    }
    return color;
}

Vector3f PathTracer::randChoice(Material const &material, Vector3f const &n, Vector3f const &v,
                    Vector3f &l, bool reverse) const {
    Vector3f w;
    for(int times = 1; times < 10; times++)
    {
        switch(rand() & 3)
        {
            case 0: // 漫反射
                if(material.diffuse < epsColor && material.specular < epsColor)
                    break;
                l = Vector3f::getRandUnit().forcePositiveBy(n);
                w = reverse? material.calcCosBRDF(l, v, n): material.calcCosBRDF(v, l, n);
                return w * (4 / times);
            case 1: // 漫透射
                if(material.tdiffuse < epsColor && material.tspecular < epsColor)
                    break;
                l = Vector3f::getRandUnit().forcePositiveBy(-n);
                w = reverse? material.calcCosBRDF(l, v, n): material.calcCosBRDF(v, l, n);
                return w * (4 / times);
            case 2: // 镜面反射
                if(material.reflection < epsColor)
                    break;
                l = World::calcReflectiveDir(v, n);
                w = material.reflection;
                return w * (4 / times);
            case 3: // 折射
                if(material.refraction < epsColor)
                    break;
                l = World::calcRefractiveDir(v, n, material.refractiveIndex, material.outRefractiveIndex);
                w = material.refraction;
                return w * (4 / times);
            default:
                assert(false);
        }
    }
    return 0;
}
