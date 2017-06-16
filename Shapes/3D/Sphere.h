//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_SPHERE_H
#define INC_2RAYTRACE_SPHERE_H

#include "Shape3D.h"

class Sphere : public Shape3D {
    Vector3f o;
    float r;
public:
    Sphere();

    Sphere(const Vector3f &o, float r);

    bool isInside(Vector3f const &point) const override;

    bool isOnSurface(Vector3f const &point) const override;

    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;

    Vector3f getUV(Vector3f const &point) const override;

    Vector3f getNormalVector(Vector3f const &param) const override;

    virtual AxisBox getAABB() const override ;
};


#endif //INC_2RAYTRACE_SPHERE_H
