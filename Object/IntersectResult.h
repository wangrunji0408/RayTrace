//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_INTERSECTRESULT_H
#define INC_2RAYTRACE_INTERSECTRESULT_H

#include <iostream>
#include "../Geometry/Vector3.h"
#include "Object.h"

class IntersectResult {
    bool success;
    Ray ray;
    Ray face;
    float t;
    Vector3f param;
    shared_ptr<Object> object;
public:
    IntersectResult();
    IntersectResult(const Ray &ray, shared_ptr<Object> object, float t, Vector3f const &normal, Vector3f const &param);

    bool isSuccess () const;
    bool isOuter () const;
    Vector3f getPoint () const;
    Vector3f getNormal() const;
    Ray getFace () const;
    const Vector3f &getParam() const;

    float getT () const;
    shared_ptr<Object> getObject() const;

    friend std::ostream &operator<<(std::ostream &os, const IntersectResult &result);

    static IntersectResult miss;
};


#endif //INC_2RAYTRACE_INTERSECTRESULT_H
