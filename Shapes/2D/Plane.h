//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_PLANE_H
#define INC_2RAYTRACE_PLANE_H

#include <ostream>
#include "Shape2D.h"
#include "../../Geometry/Transform.h"

class Plane : public Shape2D {
public:
    Ray normal;
    float size = inf;
    Transform trans;
    static int intersectCount;
public:
    Plane() {}

    Plane(const Ray &normal);

    Plane(Vector3f const &a, Vector3f const &b, Vector3f const &c);

    bool isOnSurface(Vector3f const &point) const override;

    Vector3f getNormalVector(Vector3f const &param) const override;

    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;

    Vector3f getUV(Vector3f const &point) const override;

    float dist(Vector3f const &point) const;

    AxisBox getAABB() const override;

    friend std::ostream &operator<<(std::ostream &os, const Plane &plane);
};


#endif //INC_2RAYTRACE_PLANE_H
