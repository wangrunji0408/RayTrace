//
// Created by 王润基 on 2017/6/14.
//

#ifndef INC_2RAYTRACE_BEZIERSURFACE_H
#define INC_2RAYTRACE_BEZIERSURFACE_H


#include "ParameterSurface.h"
#include "../1D/BezierCurve.h"
#include "../3D/TriangleMesh.h"

class BezierSurface: public ParameterSurface {
private:
    int m, n;
    std::vector<BezierCurve> curves;
public:
    BezierSurface(int m, int n, std::vector<Point> const& points);

    Point getControlPoint (int i, int j) const;
    Point getPoint(Vector3f const &param) const override;

    AxisBox getAABB() const override;
};


#endif //INC_2RAYTRACE_BEZIERSURFACE_H
