//
// Created by 王润基 on 2017/4/11.
//

#ifndef INC_2RAYTRACE_BEZIERLINE_H
#define INC_2RAYTRACE_BEZIERLINE_H


#include <vector>
#include "Shape1D.h"

class BezierLine: public Shape1D {
private:
    std::vector<Vector3f> points;
public:
    BezierLine(const std::vector<Vector3f> &points);
    Vector3f getPoint (float t) const;
    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;

    AxisBox getAABB() const override;
};


#endif //INC_2RAYTRACE_BEZIERLINE_H
