//
// Created by 王润基 on 2017/4/19.
//

#include <cmath>
#include "SpotLight.h"

SpotLight::SpotLight(Color const &color, const Ray &ray, float angle, float edgeAngle) :
        ray(ray), angle(angle), edgeAngle(edgeAngle)
{
    this->color = color;
}

Light SpotLight::illuminate(Vector3f const &point) const {
    Color c = Color::zero;
    float theta = acosf(ray.getUnitDir().cos(point - ray.getStartPoint()));
    if(theta < angle + edgeAngle) {
        float t = sqrtf(std::min(1.0f, (angle + edgeAngle - theta) / edgeAngle));
        c = color / (point - ray.getStartPoint()).len2() * t;
    }
    return Light(ray.getStartPoint(), point, c);
}

bool SpotLight::inRange(Vector3f const &point) const {
    return ray.getUnitDir().cos(point - ray.getStartPoint()) > cosf(angle + edgeAngle) - eps;
}

Light SpotLight::sample() const {
    auto const& pos = ray.getStartPoint();
    auto const& main = ray.getUnitDir();
    Vector3f dir;
    do {dir = Vector3f::getRandUnit();}
    while(main.angle(dir) > angle + edgeAngle);
    return Light(pos, pos + dir, color);
}
