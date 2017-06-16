//
// Created by 王润基 on 2017/4/30.
//

#include <cstring>
#include "Transform.h"
#include "Geometry.h"

using std::endl;

Transform Transform::operator*(Transform const &b) const {
    Transform c; c.clear();
    for(int i=0; i<N; ++i)
        for(int j=0; j<N; ++j)
            for(int k=0; k<N; ++k)
                c.v[i][j] += v[i][k] * b.v[k][j];
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
    auto a = Transform();
    angle = -angle;
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

void Transform::addLine(int i1, int i2, float k) {
    for(int j=0; j<N; ++j)
        v[i1][j] += k * v[i2][j];
}

void Transform::mulLine(int i, float k) {
    for(int j=0; j<N; ++j)
        v[i][j] *= k;
}

void Transform::swapLine(int i1, int i2) {
    for(int j=0; j<N; ++j)
        std::swap(v[i1][j], v[i2][j]);
}

Transform Transform::inverse() const {
    auto mat = *this, inv = Transform();
    for(int i=0; i<N-1; ++i) {
        int ii = i;
        float maxabs = 0;
        for(int j=i; j<N; ++j)
            if(fabsf(mat.v[j][i]) > maxabs)
                maxabs = fabsf(mat.v[j][i]), ii = j;
        if(maxabs < eps)
            throw std::invalid_argument("Matrix inverse not exist.");
        mat.swapLine(i, ii);
        inv.swapLine(i, ii);

        float k = 1 / mat.v[i][i] - 1;
        mat.addLine(i, i, k);
        inv.addLine(i, i, k);
        for (int j = i + 1; j < N; ++j) {
            float k = -mat.v[j][i];
            mat.addLine(j, i, k);
            inv.addLine(j, i, k);
        }
//        std::cerr << mat << inv << endl;
    }
    for(int i=N-1; i>=1; --i){
        for(int j=i-1; j>=0; --j) {
            float k = -mat.v[j][i];
            mat.addLine(j, i, k);
            inv.addLine(j, i, k);
        }
//    std::cerr << mat << inv << endl;
    }

    return inv;
}

Ray Transform::operator*(Ray const &ray) const {
    return Ray::fromTo(operator*(ray.getStartPoint()), operator*(ray.getEndPoint()));
}

Transform::Transform() {
    clear();
    for(int i=0; i<N; ++i)
        v[i][i] = 1;
}

Transform Transform::toZ01(Vector3f const &a0, Vector3f const &a1) {
    auto d = a1 - a0;
    float l = d.len();
    // 先绕x轴转到xz平面，角度phi；再绕y轴转到与z轴重合，角度theta
    float phi = (float)M_PI_2 - atan2f(d.z, d.y);
    float theta = 0 - asinf(d.x / l);
    return rotate(Vector3f(phi, theta, 0)) * scale(Vector3f(1 / l)) * move(-a0);
}

Transform Transform::scale(Vector3f const &scale) {
    auto t = Transform();
    t.v[0][0] = scale.x;
    t.v[1][1] = scale.y;
    t.v[2][2] = scale.z;
    return t;
}

std::ostream &operator<<(std::ostream &os, const Transform &transform) {
    os << "[Transform\n";
    for(int i=0; i<4; ++i) {
        for (int j = 0; j < 4; ++j)
            os << transform.v[i][j] << '\t';
        os << endl;
    }
    return os << "]" << endl;
}
