//
// Created by 王润基 on 2017/6/15.
//

#include "ParameterSurface.h"

void ParameterSurface::makeMesh(int m, int n) {
    meshm = m;
    meshn = n;
    mesh = TriangleMesh(*this, m, n);
    mesh.edge = true; // 开启三角面片边界碰撞
}

bool ParameterSurface::tryGetIntersectionPoint(Ray const &ray, float &t, Vector3f& param) const {
    bool exist = mesh.tryGetIntersectionPoint(ray, t, param);
    // 转换param格式
    param = fromMeshParam(param);
    Vector3f meshParam = param;
    float &u = param.x, &v = param.y;

    // 如果不需要迭代，在这里就可以结束了
    if(!exist || rendering != ITERATION)
        return exist;

    Vector3f dir = ray.getUnitDir();
    for(int i=0; i<iterTimes; ++i)
    {
        auto p = getPoint(param);
        auto f = ray.getEndPoint(t) - p;
        float f2 = f.len2();
//        std::cerr << "u = " << u << " v = " << v << " t = " << t << " |f|^2 = " << f2 << std::endl;
        if(f2 < eps)
            break;
        auto df = -f;
        auto pppu = getPartial(param, 0);
        auto pppv = getPartial(param, 1);
        auto d = dir.dot(pppu.det(pppv));
        t += pppu.dot(pppv.det(df)) / d;
        u += dir.dot(pppv.det(df)) / d;
        v -= dir.dot(pppu.det(df)) / d;
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

void ParameterSurface::intersect(IntersectInfo &info) const {
    if(info.testBlockT != 0) {
        info.success = testRayBlocked(info.ray, info.testBlockT);
        return;
    }
    if(rendering == ITERATION) {
        info.success = tryGetIntersectionPoint(info.ray, info.t, info.param);
        if(!info.success)   return;
        if(info.needNormal)
            info.normal = getNormalVector(info.param);
        if(info.needUV)
            info.uv = getUV(info.param);
    } else { // mesh
        mesh.intersect(info);
        info.param = fromMeshParam(info.param);
        info.uv = getUV(info.param);
    }
}

Vector3f ParameterSurface::fromMeshParam(Vector3f const &meshParam) const {
    Vector3f param;
    float &u = param.x;
    float &v = param.y;
    param.z = 0;
    auto gravity = mesh.paramToGravity(meshParam);
    int faceId = (int)meshParam.z - 1;
    if((faceId & 1) == 0)  // 左上片
    {
        u = gravity.z;
        v = gravity.y;
    }
    else    // 右下片
    {
        u = 1 - gravity.y;
        v = 1 - gravity.z;
    }
    u = (faceId/2 / meshn + u) / meshm;
    v = (faceId/2 % meshn + v) / meshn;
    return param;
}
