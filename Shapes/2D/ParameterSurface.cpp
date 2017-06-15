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
    for(int i=0; i<5; ++i)
    {
        auto p = getPoint(param);
        auto f = ray.getEndPoint(t) - p;
        float f2 = f.len2();
//        std::cerr << "u = " << u << " v = " << v << " t = " << t << " |f| = " << f2 << std::endl;
        if(f2 < eps)
        {
//            std::cerr << std::endl;
            return true;
        }
        auto df = -f;
        auto pppu = getPartial(param, 0);
        auto pppv = getPartial(param, 1);
        auto d = dir.dot(pppu.det(pppv));
        t += pppu.dot(pppv.det(df)) / d;
        param.x += dir.dot(pppv.det(df)) / d;
        param.y -= dir.dot(pppu.det(df)) / d;
    }
//    std::cerr << std::endl;
    return false;
}

Point ParameterSurface::getPartial(Vector3f const &param, int id) const {
    float d = 1e-3;
    auto p1 = param; p1.value(id) += d;
    return (getPoint(p1) - getPoint(param)) / d;
}

bool ParameterSurface::tryGetIntersectionInfo(Ray const &ray, float &t, Vector3f &point, Vector3f &normal) const {
    Vector3f param;
    bool exist =tryGetIntersectionPoint(ray, t, param);
    if(!exist)  return false;
    point = getPoint(param);
    normal = getNormalVector(param);
    return true;
}

bool ParameterSurface::isOnSurface(Vector3f const &point) const {
    return false;
}

Vector3f ParameterSurface::getNormalVectorOnSurface(Vector3f const &point) const {
    return Point::zero;
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
