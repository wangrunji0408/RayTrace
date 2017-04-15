//
// Created by 王润基 on 2017/4/11.
//

#include "BezierLine.h"

Vector3f BezierLine::getPoint(float t) const {
    std::vector<Vector3f> ps = points;
    for(int k = (int)points.size(); k > 1; --k)
        for(int i=0; i<k-1; ++i)
            ps[i] = ps[i] * (1 - t) + ps[i+1] * t;
    return ps[0];
}

BezierLine::BezierLine(const std::vector<Vector3f> &points) : points(points) {}

bool BezierLine::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    // TODO
    return false;
}
