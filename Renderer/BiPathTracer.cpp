//
// Created by 王润基 on 2017/6/21.
//

#include "BiPathTracer.h"

BiPathTracer::BiPathTracer(shared_ptr<World> world, shared_ptr<Camera> camera) : PathTracer(world, camera) {

}

Color BiPathTracer::renderRay(Ray const &ray, int depth, Color weight) const {
    std::vector<IntersectInfo> forwardPath, backwardPath;
    LightSource* lightSource = nullptr;
    sampleForwardPath(forwardPath, lightSource);
    sampleBackwardPath(ray, backwardPath);
    auto color = Color::zero;
    for(auto const& info: backwardPath)
    {
        auto const& material = info.getMaterial();
        auto const& v = -info.ray.getUnitDir();
        auto const& n = info.normal;
        auto const& point = info.getPoint();
        // Emission
        color += info.weight * material.calcEmission(v, n);
        // Direct
        {
            auto light = lightSource->illuminate(point);
            if (!world->testLightBlocked(light))
                color += info.weight * light.color * material.calcCosBRDF(-light.getUnitDir(), v, n);
        }
        // Indirect
        for(auto const& finfo: forwardPath)
        {
            if(info.weight * finfo.weight < epsColor)
                break;
            auto fpoint = finfo.getPoint();
            auto ray = Ray::fromTo(fpoint, point);
            float dist = (fpoint - point).len();
            if(world->testLightBlocked(ray, dist))
                continue;
            auto const& dir = ray.getUnitDir();
            auto weight = info.weight * material.calcCosBRDF(-dir, v, n);
            auto const& fmaterial = finfo.getMaterial();
            color += weight * fmaterial.calcEmission(dir, finfo.normal);
            weight *= fmaterial.calcCosBRDF(-finfo.ray.getUnitDir(), dir, finfo.normal);
            color += weight * finfo.weight;
        }
    }
//    static int total;
//    std::cerr << ++total << std::endl;
    int count = (int) ((forwardPath.size() + 1) * backwardPath.size());
    return color / count;
}

void BiPathTracer::sampleBackwardPath(Ray const& ray0, std::vector<IntersectInfo>& path) const {
    path.clear();
    path.reserve(10);
    Ray ray = ray0;
    Color weight = Color(1,1,1);
    for(int depth = 0; depth<maxDepth && !(weight < epsColor); depth++)
    {
        auto result = world->tryGetFirstIntersectionPoint(ray);
        if(!result.success)
            break;
        result.weight = weight;
        path.push_back(result);
        auto obj = result.object;
        auto point = result.getPoint();
        auto material = obj->getMaterialAt(result.uv);
        auto v = ray.getStartPoint() - point;
        auto n = result.normal;

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
                l = l.forcePositiveBy(n);
                weight *= 0.5;
            }
            brdf = material.calcCosBRDF(l, v, n);
        }
        ray = Ray(point + l * 1e-4, l);
        weight *= brdf;
    }
}

void BiPathTracer::sampleForwardPath(std::vector<IntersectInfo> &path, LightSource *&lightSource) const {
    path.clear();
    path.reserve(10);

    auto const& lights = world->getLights();
    auto ray = Ray();
    auto result = IntersectInfo(ray);
    auto weight = Color(1,1,1);

    for(int depth = 0; depth<maxDepth && !(weight < epsColor); depth++)
    {
        if(depth == 0)
        {
            do {
                lightSource = lights[rand() % lights.size()].get();
                ray = lightSource->sample();
                result = world->tryGetFirstIntersectionPoint(ray);
            } while(!result.success);
            auto light = lightSource->illuminate(result.getPoint());
            weight = light.color;
        }
        else
            result = world->tryGetFirstIntersectionPoint(ray);

        result.weight = weight;
        path.push_back(result);
        if(!result.success)
            break;
        auto obj = result.object;
        auto point = result.getPoint();
        auto material = obj->getMaterialAt(result.uv);
        auto l = ray.getStartPoint() - point;
        auto n = result.normal;

        Color brdf; Vector3f v;
        if(!(material.reflection < epsColor))
        {
            v = World::calcReflectiveDir(l, n);
            brdf = material.reflection;
        }
        else if(!(material.refraction < epsColor))
        {
            v = World::calcRefractiveDir(l, n, material.refractiveIndex, material.outRefractiveIndex);
            brdf = material.refraction;
        }
        else
        {
            v = Vector3f::getRandUnit();
            if(material.tdiffuse < epsColor && material.tspecular < epsColor) {
                v = v.forcePositiveBy(n);
                weight *= 0.5;
            }
            brdf = material.calcCosBRDF(l, v, n);
        }
        ray = Ray(point + v * 1e-4, v);
        weight *= brdf;
    }
}
