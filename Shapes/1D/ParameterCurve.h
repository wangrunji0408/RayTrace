//
// Created by 王润基 on 2017/6/15.
//

#ifndef INC_2RAYTRACE_PARAMETERCURVE_H
#define INC_2RAYTRACE_PARAMETERCURVE_H


#include "Shape1D.h"

class ParameterCurve : public Shape1D {
public:
    virtual Point getPoint (float t) const = 0;
};


#endif //INC_2RAYTRACE_PARAMETERCURVE_H
