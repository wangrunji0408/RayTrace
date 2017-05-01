//
// Created by 王润基 on 2017/4/30.
//

#ifndef INC_2RAYTRACE_TRANSFORM_H
#define INC_2RAYTRACE_TRANSFORM_H


#include "Vector3.h"
#include "Ray.h"

class Transform {
public:
    static const int N = 4;
    float v[N][N];
private:
    void clear ();
public:
    Transform ();
    Transform transpose () const;
    Transform inverse () const;
    Transform operator* (Transform const& b) const;
    Vector3f operator* (Vector3f const& vec) const;
    Ray operator* (Ray const& ray) const;
    static Transform move (Vector3f const& p);
    static Transform rotate (int axis, float angle);
    static Transform rotate (Vector3f const& angle);
};


#endif //INC_2RAYTRACE_TRANSFORM_H
