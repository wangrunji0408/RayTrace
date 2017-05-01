//
// Created by 王润基 on 2017/4/30.
//

#include <cstring>
#include "Transform.h"

Transform Transform::operator*(Transform const &b) const {
    Transform c; c.clear();
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            for(int k=0; k<N; ++k)
                c.v[i][j] += v[i][k] + v[k][j];
    return c;
}

void Transform::clear() {
    memset(v, 0, sizeof(v));
}

Vector3f Transform::operator*(Vector3f const &vec) const {
    Vector3f c;
    for(int i=0; i<3; ++i) {
        for (int j = 0; j < 3; ++j)
            c.value(i) += v[i][j] * vec.value(j);
        c.value(i) += v[i][3] * 1;
    }
    return c;
}

Transform Transform::move(Vector3f const &p) {
    auto a = Transform();
    for(int i=0; i<3; ++i)
        a.v[i][3] = p.value(i);
    return a;
}

Transform Transform::rotate(Vector3f const &angle) {
    return rotate(2, angle.z) * rotate(1, angle.y) * rotate(0, angle.x);
}

Transform Transform::rotate(int axis, float angle) {
    if(axis < 0 || axis >= 3)
        throw std::invalid_argument("axis id");
    auto a = Transform(); a.clear();
    float cos = cosf(angle), sin = sinf(angle);
    int i1 = (axis + 1) % 3, i2 = (axis + 2) % 3;
    a.v[axis][axis] = 1;
    a.v[i1][i1] = cos;
    a.v[i2][i2] = cos;
    a.v[i1][i2] = -sin;
    a.v[i2][i1] = sin;
    return a;
}

Transform Transform::transpose() const {
    auto a = Transform();
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            a.v[i][j] = v[j][i];
    return a;
}

Transform Transform::inverse() const {
    return Transform();
}

Ray Transform::operator*(Ray const &ray) const {
    return Ray(operator*(ray.getStartPoint()), operator*(ray.getUnitDir()));
}

Transform::Transform() {
    clear();
    for(int i=0; i<N; ++i)
        v[i][i] = 1;
}
