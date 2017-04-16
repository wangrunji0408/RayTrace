//
// Created by 王润基 on 2017/4/9.
//

#include "Camera.h"

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
