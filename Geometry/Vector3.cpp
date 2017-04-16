//
// Created by 王润基 on 2017/4/8.
//

#include "Vector3.h"
#include "Geometry.h"
#include <cmath>

template<class T>
Vector3<T> Vector3<T>::operator+(Vector3<T> const &b) const {
    return Vector3(x + b.x, y + b.y, z + b.z);
}

template<class T>
Vector3<T> Vector3<T>::operator-(Vector3<T> const &b) const {
    return Vector3(x - b.x, y - b.y, z - b.z);
}

template<class T>
Vector3<T> Vector3<T>::operator*(Vector3<T> const &b) const {
    return Vector3(x * b.x, y * b.y, z * b.z);
}

template<class T>
Vector3<T> Vector3<T>::operator/(Vector3<T> const &b) const {
    return Vector3(x / b.x, y / b.y, z / b.z);
}

template<class T>
Vector3<T> Vector3<T>::operator-() const {
    return Vector3(-x, -y, -z);
}

template<class T>
Vector3<T> Vector3<T>::operator*(T const &k) const {
    return Vector3(x * k, y * k, z * k);
}

template<class T>
Vector3<T> Vector3<T>::operator/(T const &k) const {
    return Vector3(x / k, y / k, z / k);
}

template<class T>
T Vector3<T>::dot(Vector3<T> const &a, Vector3<T> const &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<class T>
Vector3<T> Vector3<T>::det(Vector3<T> const &a, Vector3<T> const &b) {
    return Vector3(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
}

template<class T>
float Vector3<T>::len() const {
    return sqrtf(len2());
}

template<class T>
T Vector3<T>::len2() const {
    return x * x + y * y + z * z;
}

template<class T>
Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

template<class T>
Vector3<T>::Vector3(T x) : x(x), y(x), z(x) {}

template<class T>
Vector3<T> Vector3<T>::norm() const {
    return *this / len();
}

template<class T>
T Vector3<T>::dot(Vector3<T> const &b) const {
    return dot(*this, b);
}

template<class T>
Vector3<T> Vector3<T>::det(Vector3<T> const &b) const {
    return det(*this, b);
}

template<class T>
float Vector3<T>::angle(Vector3<T> const &a, Vector3<T> const &b) {
    return acosf(dot(a.norm(), b.norm()));
}

template<class T>
float Vector3<T>::angle(Vector3<T> const &b) const {
    return angle(*this, b);
}

template<class T>
bool Vector3<T>::isVertical(Vector3<T> const &b) const {
    return isZero(dot(b));
}

template <>
bool Vector3<int>::operator<(const Vector3<int> &rhs) const {
    return x < rhs.x &&
           y < rhs.y &&
           z < rhs.z;
}

template <>
bool Vector3<float>::operator<(const Vector3<float> &rhs) const {
    return x < rhs.x - eps &&
           y < rhs.y - eps &&
           z < rhs.z - eps;
}

template <>
bool Vector3<int>::operator==(const Vector3<int> &rhs) const {
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z;
}

template <>
bool Vector3<float>::operator==(const Vector3<float> &rhs) const {
    return isEqual(x, rhs.x) && isEqual(y, rhs.y) && isEqual(z, rhs.z);
}

template <class T>
bool Vector3<T>::operator!=(const Vector3<T> &rhs) const {
    return !(rhs == *this);
}

template <class T>
T Vector3<T>::sum() const {
    return x + y + z;
}

template <class T>
Vector3<T>::Vector3(): Vector3(0, 0, 0) {}

template <class T>
T Vector3<T>::value(int id) const {
    return (&x)[id];
}

template <class T>
void Vector3<T>::operator+=(Vector3<T> const &b) {
    *this = operator+(b);
}

template class Vector3<int>;
template class Vector3<float>;
//template class Vector3<double>;