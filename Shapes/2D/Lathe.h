//
// Created by 王润基 on 2017/6/15.
//

#ifndef INC_2RAYTRACE_LATHE_H
#define INC_2RAYTRACE_LATHE_H


#include "ParameterSurface.h"
#include "../1D/ParameterCurve.h"
#include "../../Geometry/Transform.h"

class Lathe: public ParameterSurface {
    shared_ptr<ParameterCurve> curve;
    Transform trans, transi; // 经变换后绕z轴旋转
public:
    Lathe(shared_ptr<ParameterCurve> const& curve, Ray const& axis);
    Point getPoint(Vector3f const &param) const override;
};


#endif //INC_2RAYTRACE_LATHE_H
