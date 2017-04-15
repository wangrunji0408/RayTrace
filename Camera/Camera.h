//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_CAMERA_H
#define INC_2RAYTRACE_CAMERA_H

#include <opencv2/opencv.hpp>
#include "../Geometry/Ray.h"

class Camera {
    Ray ray;
    Vector3f up, right;    // UnitVector
    int width, height;
    float focalLength;
public:
    Camera(const Vector3f &pos, const Vector3f &target, const Vector3f &up, int width, int height, float focalLength);
    int getWidth() const;
    int getHeight() const;
    void setPosition (Vector3f const& pos, Vector3f const& target, Vector3f const& up);
    void setResolution (int width, int height);
    void setFocalLength (float f);

    virtual Ray getRay (int x, int y) const;
};


#endif //INC_2RAYTRACE_CAMERA_H
