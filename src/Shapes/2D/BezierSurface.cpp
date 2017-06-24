//
// Created by 王润基 on 2017/6/14.
//

#include "BezierSurface.h"
#include "../3D/AxisBox.h"

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

BezierSurface::BezierSurface(int m, int n, std::vector<Point> const &points):
    m(m), n(n)
{
    curves.reserve(size_t(m));
    for(int i=0; i<m; ++i)
        curves.push_back(BezierCurve(std::vector<Point>(points.begin() + i*n, points.begin() + i*n+n)));
}

Point BezierSurface::getPoint(Vector3f const &param) const {
    std::vector<Point> pxs((size_t)m);
    for(int i=0; i<m; ++i)
        pxs[i] = curves[i].getPoint(param.y);
    return BezierCurve(pxs).getPoint(param.x);
}



