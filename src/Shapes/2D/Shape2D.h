//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_OBJECT2D_H
#define INC_2RAYTRACE_OBJECT2D_H


#include "../Shape.h"

class Shape2D: public Shape {
public:
    virtual bool isOnSurface (Vector3f const& point) const = 0;
    void intersect(IntersectInfo &info) const override;
};


#endif //INC_2RAYTRACE_OBJECT2D_H
