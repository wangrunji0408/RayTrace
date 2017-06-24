//
// Created by 王润基 on 2017/4/9.
//

#include "Camera.h"
#include "../Shapes/2D/Plane.h"

void Camera::setResolution(int width, int height) {
    this->width = width;
    this->height = height;
}

Ray Camera::getRay(float x, float y) const {
    auto delta = calcDelta(x, y);
    if(orthographic)
        return Ray(pos + delta, target - pos);
    return Ray::fromTo(pos, target + delta);
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
    up = upPoint.norm();
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
    float t = Plane::intersectWithPlane(Ray::fromTo(target, pos),ray);
    Point point = ray.getEndPoint(t);
    int x = height / 2 - (int)(Ray(target, up).calcProjectionT(point) / realw * width);
    int y = width / 2 + (int)(Ray(target, right).calcProjectionT(point) / realw * width);
//    std::cerr << getRay(x, y).calcDist(p) << std::endl;
    return Vector3i(x, y, 0);
}

Ray Camera::getRandRay(float x, float y) const {
    if(aperture == 0)
        return getRay(x, y);
    float dx, dy;
    do{
        dx = rand01() * 2 - 1;
        dy = rand01() * 2 - 1;
    }while(dx * dx + dy * dy > 1);
    auto delta = calcDelta(x, y);
    auto pt = target - pos;
    auto focalPoint = pos + (pt + delta) * (focalLength / pt.len());
    auto pos1 = pos + (up * dx + right * dy) * aperture;
    return Ray::fromTo(pos1, focalPoint);
}

float Camera::getAperture() const {
    return aperture;
}

void Camera::setAperture(float aperture) {
    Camera::aperture = aperture;
}

float Camera::getFocalLength() const {
    return focalLength;
}

void Camera::setFocalLength(float focalLength) {
    Camera::focalLength = focalLength;
}

Vector3f Camera::calcDelta(float x, float y) const {
    return (up * (height / 2 - x) + right * (y - width / 2)) * (realw / width);
}
