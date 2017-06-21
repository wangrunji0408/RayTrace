//
// Created by 王润基 on 2017/4/30.
//

#ifndef INC_2RAYTRACE_TRANSFORM_H
#define INC_2RAYTRACE_TRANSFORM_H


#include <ostream>
#include "Vector3.h"
#include "Ray.h"

class Transform {
public:
    static const int N = 4;
    bool isEye = true;
    float v[N][N];
private:
    void clear ();
    void addLine (int i1, int i2, float k);
    void mulLine (int i, float k);
    void swapLine (int i1, int i2);
    void updateEyeFlag ();
public:
    Transform ();
    Transform transpose () const;
    Transform inverse () const;
    bool operator== (Transform const& b) const;
    Transform operator* (Transform const& b) const;
    Vector3f operator* (Vector3f const& vec) const;
    Vector3f apply_xy (Vector3f const& vec) const;
    Ray operator* (Ray const& ray) const;
    static Transform move (Vector3f const& p);
    static Transform rotate (int axis, float angle); // 以坐标轴正方向右手螺旋为旋转方向
    static Transform rotate (Vector3f const& angle);
    static Transform scale (Vector3f const& scale);
    static Transform toZ01 (Vector3f const& a0, Vector3f const& a1);
    friend std::ostream &operator<<(std::ostream &os, const Transform &transform);
};


#endif //INC_2RAYTRACE_TRANSFORM_H
