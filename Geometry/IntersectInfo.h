//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_INTERSECTRESULT_H
#define INC_2RAYTRACE_INTERSECTRESULT_H

#include <iostream>
#include "Ray.h"

class Object;

struct IntersectInfo {
    // Request
    Ray ray;
    float testBlockT = 0; // 若此数不为0，则直接给success赋值：是否存在交点使得 t < testBlockT
    bool needNormal = false;
    bool needParam = false;
    bool needObject = false;
    bool needUV = false;

    // Response
    bool success = false;
    float t = std::numeric_limits<float>::max() / 8;
    Vector3f normal, param, uv;
    std::shared_ptr<Object> object;

    // Methods
    IntersectInfo(const Ray &ray);
    bool isOuter () const;
    Vector3f getPoint() const;
    friend std::ostream &operator<<(std::ostream &os, const IntersectInfo &info);
};


#endif //INC_2RAYTRACE_INTERSECTRESULT_H
