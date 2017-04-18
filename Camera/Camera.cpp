//
// Created by 王润基 on 2017/4/9.
//

#include "Camera.h"
#include "../Shapes/2D/Plane.h"

void Camera::setResolution(int width, int height) {
    this->width = width;
    this->height = height;
}

Ray Camera::getRay(int x, int y) const {
    auto delta = (up * (height / 2 - x) + right * (y - width / 2)) * (realw / width);
    if(orthographic)
        return Ray(pos + delta, target - pos);
    return Ray(pos, target - pos + delta);
}

Camera::Camera(const Vector3f &pos, const Vector3f &target, const Vector3f &up, int width, int height,
               float realw, bool orthographic)
{
    setPosition(pos, target, up);
    setResolution(width, height);
    setRealw(realw);
    setOrthographic(orthographic);
}

void Camera::setPosition(Vector3f const &pos, Vector3f const &target, Vector3f const &upPoint) {
    this->pos = pos;
    this->target = target;
    Ray ray = Ray::fromTo(pos, target);
    up = (upPoint - ray.calcProjectionPoint(upPoint)).norm();
    right = ray.getUnitDir().det(up);
}

int Camera::getWidth() const {
    return width;
}

int Camera::getHeight() const {
    return height;
}

bool Camera::isOrthographic() const {
    return orthographic;
}

void Camera::setOrthographic(bool orthographic) {
    Camera::orthographic = orthographic;
}

float Camera::getRealw() const {
    return realw;
}

void Camera::setRealw(float realw) {
    Camera::realw = realw;
}

Vector3i Camera::getPos(Vector3f const &p) const {
    if(p == pos) return Vector3i(height/2, width/2, 0);
    auto ray = orthographic?
               Ray(pos, pos - target):
               Ray::fromTo(pos, p);
    float t;
    Plane(Ray::fromTo(target, pos)).tryGetIntersectionPoint(ray, t);
    Point point = ray.getEndPoint(t);
    int x = height / 2 - (int)(Ray(target, up).calcProjectionT(point) / realw * width);
    int y = width / 2 + (int)(Ray(target, right).calcProjectionT(point) / realw * width);
//    std::cerr << getRay(x, y).calcDist(p) << std::endl;
    return Vector3i(x, y, 0);
}
