//
// Created by 王润基 on 2017/4/11.
//

#ifndef INC_2RAYTRACE_BEZIERCURVE_H
#define INC_2RAYTRACE_BEZIERCURVE_H


#include <vector>
#include "Shape1D.h"
#include "ParameterCurve.h"

class BezierCurve: public ParameterCurve {
    std::vector<Vector3f> points;
public:
    BezierCurve(const std::vector<Vector3f> &points);
    Point getControlPoint (int i) const;
    AxisBox getAABB() const override;
    Point getPoint(float t) const override;
};


#endif //INC_2RAYTRACE_BEZIERCURVE_H
