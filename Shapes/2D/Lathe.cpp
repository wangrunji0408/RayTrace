//
// Created by 王润基 on 2017/6/15.
//

#include "Lathe.h"


Point Lathe::getPoint(Vector3f const &param) const {
    const float pi2 = 2 * (float)M_PI;
    return transi * (Transform::rotate(2, param.y * pi2) * (trans * curve->getPoint(param.x)));
}

Lathe::Lathe(shared_ptr<ParameterCurve> const& curve, Ray const& axis) : curve(curve)
{
    trans = Transform::toZ01(axis.getStartPoint(), axis.getEndPoint());
    transi = trans.inverse();
}

