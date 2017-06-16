//
// Created by 王润基 on 2017/6/15.
//

#ifndef INC_2RAYTRACE_PARAMETERSURFACE_H
#define INC_2RAYTRACE_PARAMETERSURFACE_H


#include "Shape2D.h"
#include "../3D/TriangleMesh.h"

class ParameterSurface: public Shape2D {
public:
    int meshm, meshn;
    TriangleMesh mesh;
    enum RenderType {MESH, INTERPOLATION, ITERATION};
    RenderType rendering;
    int iterTimes = 5;
public:
    void makeMesh(int m, int n);
    void setRendering(std::string name);
    Vector3f getUV(Vector3f const &point) const override;
    bool isOnSurface(Vector3f const &point) const override; // null
    Vector3f getNormalVector(Vector3f const &param) const override;

    bool testRayBlocked(Ray const &ray, float tmin) const override; // 直接调用mesh
    bool tryGetIntersectionPoint(Ray const &ray, float &t, Vector3f &param) const override;

    virtual Point getPoint(Vector3f const& param) const = 0;
    virtual Point getPartial(Vector3f const& param, int id) const;
};


#endif //INC_2RAYTRACE_PARAMETERSURFACE_H
