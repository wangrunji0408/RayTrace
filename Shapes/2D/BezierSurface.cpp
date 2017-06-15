//
// Created by 王润基 on 2017/6/14.
//

#include "BezierSurface.h"
#include "../3D/AxisBox.h"

Point BezierSurface::getPoint(float u, float v) const {
    std::vector<Point> pxs((size_t)m);
    for(int i=0; i<m; ++i)
        pxs[i] = curves[i].getPoint(v);
    return BezierCurve(pxs).getPoint(u);
}

bool BezierSurface::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    // TODO
    return false;
}

AxisBox BezierSurface::getAABB() const {
    std::vector<Point> points(size_t(m * n));
    int k = 0;
    for(int i=0; i<m; ++i)
        for(int j=0; j<n; ++j)
            points[k++] = getControlPoint(i, j);
    return AxisBox(points.data(), points.size());
}

Point BezierSurface::getControlPoint(int i, int j) const {
    return curves[i].getControlPoint(j);
}

Point BezierSurface::getNormalVector(float u, float v) const {
    // TODO 公式求法向量
    float d = 1e-3;
    Point p0 = getPoint(u, v);
    Point pu = (getPoint(u + d, v) - p0) / d;
    Point pv = (getPoint(u, v + d) - p0) / d;
    return pu.det(pv);
}

BezierSurface::BezierSurface(int m, int n, std::vector<Point> const &points):
    m(m), n(n)
{
    curves.reserve(size_t(m));
    for(int i=0; i<m; ++i)
        curves.push_back(BezierCurve(std::vector<Point>(points.begin() + i*n, points.begin() + i*n+n)));
}

bool BezierSurface::isOnSurface(Vector3f const &point) const {
    // TODO
    return false;
}

Vector3f BezierSurface::getNormalVectorOnSurface(Vector3f const &point) const {
    // TODO
    return Point::zero;
}

bool BezierSurface::tryGetIntersectionInfo(Ray const &ray, float &t, Vector3f &point, Vector3f &normal) const {
//        return mesh.tryGetIntersectionInfo(ray, t, point, normal);

    int faceId;
    float u, v;
    bool exist = mesh.tryGetIntersectionInfo(ray, t, faceId, u, v);
    if(!exist)
        return false;

    faceId--;
    u = (faceId/2 / meshn + u) / meshm;
    v = (faceId/2 % meshn + v) / meshn;
    Vector3f dir = ray.getUnitDir();
    for(int i=0; i<5; ++i)
    {
        auto p = getPoint(u, v);
        auto f = ray.getEndPoint(t) - p;
        float f2 = f.len2();
//        std::cerr << "u = " << u << " v = " << v << " t = " << t << " |f| = " << f2 << std::endl;
        if(f2 < eps)
        {
//            std::cerr << std::endl;
            point = ray.getEndPoint(t);
            normal = getNormalVector(u, v);
            return true;
        }
        auto df = -f;
        auto pppu = (getPoint(u + 1e-3f, v) - p) / 1e-3f;
        auto pppv = (getPoint(u, v + 1e-3f) - p) / 1e-3f;
        auto d = dir.dot(pppu.det(pppv));
        t += pppu.dot(pppv.det(df)) / d;
        u += dir.dot(pppv.det(df)) / d;
        v -= dir.dot(pppu.det(df)) / d;
    }
//    std::cerr << std::endl;
    return false;
}

void BezierSurface::makeMesh(int m, int n) {
    meshm = m;
    meshn = n;
    mesh = TriangleMesh(*this, m, n);
}

