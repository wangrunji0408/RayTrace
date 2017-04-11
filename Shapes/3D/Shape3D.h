//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_OBJECT3D_H
#define INC_2RAYTRACE_OBJECT3D_H


#include "../Shape.h"
#include "../2D/Shape2D.h"

class Shape3D: public Shape2D {
public:
    virtual bool isInside (Vector3f const& point) const = 0;
};


#endif //INC_2RAYTRACE_OBJECT3D_H
