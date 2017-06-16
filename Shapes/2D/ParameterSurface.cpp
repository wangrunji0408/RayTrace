//
// Created by 王润基 on 2017/6/15.
//

#include "ParameterSurface.h"

void ParameterSurface::makeMesh(int m, int n) {
    meshm = m;
    meshn = n;
    mesh = TriangleMesh(*this, m, n);
}

bool ParameterSurface::tryGetIntersectionPoint(Ray const &ray, float &t, Vector3f& param) const {
    int faceId;
    bool exist = mesh.tryGetIntersectionInfo(ray, t, faceId, param.x, param.y);
    if(!exist)
        return false;

    faceId--;
    param.x = (faceId/2 / meshn + param.x) / meshm;
    param.y = (faceId/2 % meshn + param.y) / meshn;
    Vector3f dir = ray.getUnitDir();
    for(int i=0; i<iterTimes; ++i)
    {
        auto p = getPoint(param);
        auto f = ray.getEndPoint(t) - p;
        float f2 = f.len2();
//        std::cerr << "u = " << param.x << " v = " << param.y << " t = " << t << " |f|^2 = " << f2 << std::endl;
        if(f2 < eps)
            break;
        auto df = -f;
        auto pppu = getPartial(param, 0);
        auto pppv = getPartial(param, 1);
        auto d = dir.dot(pppu.det(pppv));
        t += pppu.dot(pppv.det(df)) / d;
        param.x += dir.dot(pppv.det(df)) / d;
        param.y -= dir.dot(pppu.det(df)) / d;
    }
    bool succ =  Vector3f(0,0,-1) < param && param < Vector3f(1,1,1) && t > 0;
//    std::cerr << (succ? "": "failed!!!") << std::endl;
    return succ;
}

Point ParameterSurface::getPartial(Vector3f const &param, int id) const {
    float d = 1e-3;
    auto p1 = param; p1.value(id) += d;
    return (getPoint(p1) - getPoint(param)) / d;
}

bool ParameterSurface::tryGetIntersectionInfo(Ray const &ray, float &t, Vector3f &param, Vector3f &normal) const {
    if(rendering == ITERATION) {
        bool exist = tryGetIntersectionPoint(ray, t, param);
        if (!exist) return false;
        normal = getNormalVector(param);
        return true;
    }
    else
        return mesh.tryGetIntersectionInfo(ray, t, param, normal);
}

bool ParameterSurface::isOnSurface(Vector3f const &point) const {
    return false;
}

Point ParameterSurface::getNormalVector(Vector3f const &param) const {
    return getPartial(param, 0).det(getPartial(param, 1));
}

Vector3f ParameterSurface::getUV(Vector3f const &point) const {
    return point;
}

bool ParameterSurface::testRayBlocked(Ray const &ray, float tmin) const {
    return mesh.testRayBlocked(ray, tmin);
}

void ParameterSurface::setRendering(std::string name) {
    if(name == "mesh")
    {
        rendering = MESH;
        mesh.normalInterpolation = false;
    }
    else if(name == "interpolation")
    {
        rendering = INTERPOLATION;
        mesh.normalInterpolation = true;
    }
    else if(name == "iteration")
        rendering = ITERATION;
    else
        throw std::invalid_argument("No such rendering type: " + name);
}
