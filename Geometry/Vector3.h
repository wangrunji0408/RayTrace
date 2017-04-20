//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_VECTOR3_H
#define INC_2RAYTRACE_VECTOR3_H

#include <iostream>
#include <opencv2/opencv.hpp>

template <class T>
class Vector3 {
public:
    T x, y, z;

    Vector3();
    Vector3(T x);
    Vector3(T x, T y, T z);

    T& value (int id);
    T const& value (int id) const;
    float len () const;
    T len2 () const;
    T sum () const;
    Vector3 norm () const;
    Vector3 operator- () const;
    Vector3 operator+ (Vector3 const& b) const;
    Vector3 operator- (Vector3 const& b) const;
    Vector3 operator* (Vector3 const& b) const;
    Vector3 operator/ (Vector3 const& b) const;
    Vector3 operator* (T const& k) const;
    Vector3 operator/ (T const& k) const;
    void operator += (Vector3 const& b);
    bool operator<(const Vector3 &rhs) const;
    bool operator==(const Vector3 &rhs) const;
    bool operator!=(const Vector3 &rhs) const;

    operator cv::Scalar () const;
    operator cv::Vec<T,3> () const;

    T dot (Vector3 const& b) const;
    Vector3 det (Vector3 const& b) const;
    float angle (Vector3 const& b) const;
    float cos (Vector3 const& b) const;
    bool isVertical (Vector3 const& b) const;
//    float getProjection (Vector3 const& b) const;

    static float angle (Vector3 const& a, Vector3 const& b);
    static T dot (Vector3 const& a, Vector3 const& b);
    static Vector3 det (Vector3 const& a, Vector3 const& b);
    static Vector3 getRandUnit ();

    friend std::ostream& operator<<(std::ostream &out, Vector3<T> const& p)
    {
        return out << '(' << p.x << ',' << p.y << ',' << p.z << ')';
    }

    const static Vector3<T> zero;
};

template <class T>
const Vector3<T> Vector3<T>::zero = Vector3<T>(0, 0, 0);

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

#endif //INC_2RAYTRACE_VECTOR3_H
