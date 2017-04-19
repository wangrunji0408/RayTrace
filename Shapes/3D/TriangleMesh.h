//
// Created by 王润基 on 2017/4/18.
//

#ifndef INC_2RAYTRACE_TRIANGLEMESH_H
#define INC_2RAYTRACE_TRIANGLEMESH_H


#include "Shape3D.h"
#include "../2D/Triangle.h"
#include "Sphere.h"
#include <vector>
#include <ostream>

class TriangleMesh: public Shape3D {
    struct TriFace {
        int v[3] = {0};
        int vn[3] = {0};
        int vt[3] = {0};
    };
public:
    std::vector<Point> vs;
    std::vector<Point> vns;
    std::vector<Point> vts;
    std::vector<TriFace> faces;
    Sphere sphere;
    Triangle toTriangle(int faceId) const;
public:
    TriangleMesh(){}
    TriangleMesh(std::string file);

    void loadFromObj (std::istream& in);
    void loadFromObj (std::string file);
    void buildSphere ();
    bool tryIntersect (Ray const &ray) const;

    bool tryGetIntersectionPoint(Ray const &ray, float &t) const override;
    bool isOnSurface(Vector3f const &point) const override;
    int getFaceId(Vector3f const &point) const;
    Vector3f getNormalVectorOnSurface(Vector3f const &point) const override;
    bool isInside(Vector3f const &point) const override;

    friend std::ostream &operator<<(std::ostream &os, const TriangleMesh &mesh);
};


#endif //INC_2RAYTRACE_TRIANGLEMESH_H
