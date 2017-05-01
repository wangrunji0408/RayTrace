//
// Created by 王润基 on 2017/4/8.
//

#include "ParallelLight.h"

Light ParallelLight::illuminate(Vector3f const &point) const {
    float t = ray.calcProjectionT(point);
    float d = ray.calcDist(point);
    if(d > r || t < 0)  return Light(point, point, Vector3f::zero);
    return Light(point - ray.getUnitDir() * t, point, color);
}

ParallelLight::ParallelLight(const Ray &ray, float r, const Color &color) : ray(ray), r(r)
{
    this->color = color;
}

const Ray &ParallelLight::getRay() const {
    return ray;
}

void ParallelLight::setRay(const Ray &ray) {
    ParallelLight::ray = ray;
}

const Color &ParallelLight::getColor() const {
    return color;
}

void ParallelLight::setColor(const Color &color) {
    ParallelLight::color = color;
}

Light ParallelLight::sample() const {
    // TODO
    throw std::exception();
}
