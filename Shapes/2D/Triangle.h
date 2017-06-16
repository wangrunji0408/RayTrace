//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_TRIANGLE_H
#define INC_2RAYTRACE_TRIANGLE_H


#include "Shape2D.h"
#include "Plane.h"

class Triangle: public Shape2D {
public:
    Vector3f a, b, c;
    static int intersectCount;
//    Plane plane;
public:
    Triangle() {}

    bool testRayBlocked(Ray const &ray, float tmin) const override;

    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c);
    Triangle(const Vector3f points[]);

    bool isOnSurface(Vector3f const &point) const override;
    Vector3f getNormalVector(Vector3f const &param) const override;
    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;

    Plane getPlane() const;

    Vector3f getUV(Vector3f const &point) const override;

    Vector3f calcGravityCoordinate (Vector3f const &point) const;

    virtual AxisBox getAABB() const override ;
};


#endif //INC_2RAYTRACE_TRIANGLE_H
