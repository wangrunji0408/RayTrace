//
// Created by 王润基 on 2017/6/14.
//

#ifndef INC_2RAYTRACE_BEZIERSURFACE_H
#define INC_2RAYTRACE_BEZIERSURFACE_H


#include "Shape2D.h"
#include "../1D/BezierCurve.h"
#include "../3D/TriangleMesh.h"

class BezierSurface: public Shape2D {
private:
    int m, n, meshm, meshn;
    std::vector<BezierCurve> curves;
    bool useMesh = false;
    TriangleMesh mesh;
public:
    BezierSurface(int m, int n, std::vector<Point> const& points);
    void makeMesh(int m, int n);

    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;
    bool tryGetIntersectionInfo(Ray const &ray, float &t, Vector3f &point, Vector3f &normal) const override;
    bool isOnSurface(Vector3f const &point) const override;
    Vector3f getNormalVectorOnSurface(Vector3f const &point) const override;

    Point getControlPoint (int i, int j) const;
    Point getPoint (float u, float v) const;
    Point getNormalVector (float u, float v) const;

    AxisBox getAABB() const override;
};


#endif //INC_2RAYTRACE_BEZIERSURFACE_H
