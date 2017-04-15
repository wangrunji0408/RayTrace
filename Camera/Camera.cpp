//
// Created by 王润基 on 2017/4/9.
//

#include "Camera.h"

void Camera::setResolution(int width, int height) {
    this->width = width;
    this->height = height;
}

Ray Camera::getRay(int x, int y) const {
    auto dir = ray.getUnitDir() * focalLength
               + up * (height / 2 - x)
               + right * (y - width / 2);
    return Ray(ray.getStartPoint(), dir);
}

Camera::Camera(const Vector3f &pos, const Vector3f &target, const Vector3f &up, int width, int height,
               float focalLength)
{
    setPosition(pos, target, up);
    setResolution(width, height);
    setFocalLength(focalLength);
}

void Camera::setPosition(Vector3f const &pos, Vector3f const &target, Vector3f const &upPoint) {
    ray = Ray::fromTo(pos, target);
    up = (upPoint - ray.calcProjectionPoint(upPoint)).norm();
    right = ray.getUnitDir().det(up);
}

void Camera::setFocalLength(float f) {
    focalLength = f;
}

int Camera::getWidth() const {
    return width;
}

int Camera::getHeight() const {
    return height;
}
