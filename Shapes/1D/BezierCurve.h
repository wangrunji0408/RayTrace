//
// Created by 王润基 on 2017/4/11.
//

#ifndef INC_2RAYTRACE_BEZIERCURVE_H
#define INC_2RAYTRACE_BEZIERCURVE_H


#include <vector>
#include "Shape1D.h"

class BezierCurve: public Shape1D {
private:
    std::vector<Vector3f> points;
public:
    BezierCurve(const std::vector<Vector3f> &points);
    Point getControlPoint (int i) const;
    Point getPoint (float t) const;
    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;

    AxisBox getAABB() const override;
};


#endif //INC_2RAYTRACE_BEZIERCURVE_H
