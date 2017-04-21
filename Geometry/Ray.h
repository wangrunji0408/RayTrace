//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_RAY_H
#define INC_2RAYTRACE_RAY_H


#include "Vector3.h"

class Ray {
private:
    Vector3f start, unitdir;
public:
    Ray();
    Ray(const Vector3f &start, const Vector3f &direction);
    static Ray fromTo (Vector3f const& from, Vector3f const& to);
    Vector3f getStartPoint() const;
    Vector3f getEndPoint(float t) const;
    Vector3f getEndPoint() const;
    Vector3f getUnitDir() const;
    float calcDist(Vector3f const &p) const;
    float calcProjectionT(Vector3f const &p) const;
    Vector3f calcProjectionPoint(Vector3f const &p) const;
    Vector3f calcSymmetryPoint (Vector3f const& p) const;

    friend std::ostream& operator<< (std::ostream& out, Ray const& ray);
};

#endif //INC_2RAYTRACE_RAY_H
