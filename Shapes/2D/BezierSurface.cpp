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
    Point pu = getPoint(u + d, v) - p0;
    Point pv = getPoint(u, v + d) - p0;
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

