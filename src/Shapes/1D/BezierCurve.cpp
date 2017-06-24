//
// Created by 王润基 on 2017/4/11.
//

#include "BezierCurve.h"
#include "../3D/AxisBox.h"

Point BezierCurve::getPoint(float t) const {
    std::vector<Vector3f> ps = points;
    for(int k = (int)points.size(); k > 1; --k)
        for(int i=0; i<k-1; ++i)
            ps[i] = ps[i] * (1 - t) + ps[i+1] * t;
    return ps[0];
}

BezierCurve::BezierCurve(const std::vector<Vector3f> &points) : points(points) {}

AxisBox BezierCurve::getAABB() const {
    return AxisBox(points.data(), points.size());
}

Point BezierCurve::getControlPoint(int i) const {
    return points[i];
}
