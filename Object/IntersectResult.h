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
    Object* object;
public:
    IntersectResult();
    IntersectResult(const Ray &ray, Object *object, float t, Vector3f const& normal);

    bool isSuccess () const;
    bool isOuter () const;
    Vector3f getPoint () const;
    Vector3f getNormal() const;
    Ray getFace () const;
    float getT () const;
    Object* getObject() const;

    friend std::ostream &operator<<(std::ostream &os, const IntersectResult &result);

    static IntersectResult miss;
};


#endif //INC_2RAYTRACE_INTERSECTRESULT_H
