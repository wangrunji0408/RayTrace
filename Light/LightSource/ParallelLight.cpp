//
// Created by 王润基 on 2017/4/8.
//

#include "ParallelLight.h"

Light ParallelLight::illuminate(Vector3f const &point) const {
    float t = ray.calcProjectionT(point);
    Color color0 = t > 0? color: Vector3f::zero;
    return Light(point - ray.getUnitDir() * t, point, color0);
}

ParallelLight::ParallelLight(const Ray &ray, const Color &color) : ray(ray)
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
