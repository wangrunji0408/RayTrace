//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_PLANE_H
#define INC_2RAYTRACE_PLANE_H

#include "Shape2D.h"

class Plane: public Shape2D{
public:
    Ray normal;
public:
    Plane(const Ray &normal);
    Plane(Vector3f const& a, Vector3f const& b, Vector3f const& c);

    bool isOnSurface(Vector3f const &point) const override;
    Vector3f getNormalVectorOnSurface(Vector3f const &point) const override;
    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;

    float dist (Vector3f const& point) const;
};


#endif //INC_2RAYTRACE_PLANE_H
