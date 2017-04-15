//
// Created by 王润基 on 2017/4/15.
//

#include "LightProjection.h"

cv::Vec3f LightProjection::renderPixel(int x, int y) const {
    auto ray = camera->getRay(x, y);
    auto result = world->tryGetFirstIntersectionPoint(ray);
    if(!result.isSuccess())
        return cv::Vec3f(0, 0, 0);
    auto obj = result.getObject();
    auto point = result.getPoint();
    Color color = Vector3f::zero;
    for(auto light : world->getLights())
    {
        Light l = light->illuminate(point);
        Color f = obj->getMaterial()->calcF(-l.getUnitDir(), ray.getStartPoint() - point, result.getNormal());
        color += l.color * f;
    }
//    std::cerr << x << ' ' << y << ' ' << color << std::endl;
    return cv::Vec3f(color.x, color.y, color.z);
}

LightProjection::LightProjection(World *world, Camera *camera) : Renderer(world, camera) {}
