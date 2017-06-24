//
// Created by 王润基 on 2017/4/9.
//

#ifndef INC_2RAYTRACE_CAMERA_H
#define INC_2RAYTRACE_CAMERA_H

#include <opencv2/opencv.hpp>
#include "../Geometry/Ray.h"

class Camera {
public:
    std::string name;
private:
    Vector3f pos, target;
    Vector3f up, right;    // UnitVector
    float realw;
    float aperture = 0;
    int width, height;
    float focalLength;
    bool orthographic;

    Vector3f calcDelta(float x, float y) const;

public:
    Camera(const Vector3f &pos, const Vector3f &target, const Vector3f &up, int width, int height,
           float realw, bool orthographic);

    int getWidth() const;

    int getHeight() const;

    float getRealw() const;

    float getAperture() const;

    float getFocalLength() const;

    void setFocalLength(float focalLength);

    void setAperture(float aperture);

    void setPosition(Vector3f const &pos, Vector3f const &target, Vector3f const &up);

    void setResolution(int width, int height);

    void setRealw(float realw);

    bool isOrthographic() const;

    void setOrthographic(bool orthographic);

    Vector3i getPos(Vector3f const &p) const;

    virtual Ray getRay(float x, float y) const;

    virtual Ray getRandRay(float x, float y) const;
};


#endif //INC_2RAYTRACE_CAMERA_H
