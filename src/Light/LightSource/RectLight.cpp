//
// Created by wangrunji0408 on 2017/6/22.
//

#include "RectLight.h"

Light RectLight::illuminate(Vector3f const &point) const {
    throw std::exception();
}

Ray RectLight::sample() const {
    throw std::exception();
}

std::vector<Light> RectLight::illuminates(Vector3f const &point) const {
    auto lights = std::vector<Light>();
    int const total = sampleSize * sampleSize;
    lights.reserve((unsigned) total);
    auto normal = plane.getNormalVector(0);
    for(int i=0; i<sampleSize; ++i)
        for(int j=0; j<sampleSize; ++j)
        {
            float u = (float(i*2+1) / sampleSize - 1) * plane.size;
            float v = (float(j*2+1) / sampleSize - 1) * plane.size;
            auto pos = plane.getPoint(Vector3f(u, v, 0));
            float cos = (point - pos).norm().dot(normal);
            if(cos < eps) continue;
            auto light = Light(pos, point, color * cos / total);
            lights.push_back(light);
        }
    return lights;
}

RectLight::RectLight(Color const& color, const Ray &normal, float size) :
        plane(normal)
{
    this->color = color;
    plane.size = size;
}
