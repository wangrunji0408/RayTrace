//
// Created by 王润基 on 2017/4/18.
//

#ifndef INC_2RAYTRACE_TRIANGLEMESH_H
#define INC_2RAYTRACE_TRIANGLEMESH_H


#include "Shape3D.h"
#include "../2D/Triangle.h"
#include "Sphere.h"
#include "AxisBox.h"
#include "../../DataStructure/KDTree.h"
#include "../../DataStructure/AABBTree.h"
#include <vector>
#include <ostream>

class ParameterSurface;

class TriangleMesh: public Shape3D {
public:
    struct TriFace {
        int v[3] = {0};
        int vn[3] = {0};
        int vt[3] = {0};
        TriFace(){}
        TriFace(int i, int j, int k);
        void swap (int i, int j);
        friend std::ostream &operator<<(std::ostream &os, const TriFace &face);
    };
public:
    int cutSize = 1 << 28;
    bool normalInterpolation = false;
    std::vector<Point> vs;
    std::vector<Point> vns;
    std::vector<Point> vts;
    std::vector<TriFace> faces;
    std::vector<Triangle> triangles;
    std::vector<std::vector<int> > faceIdsInSpace;
    bool edge = false;
    AxisBox boundingBox;
    KDTree kdTree;
    AABBTree aabbTree;
    Triangle toTriangle(int faceId) const;
    int calcSpaceId (int faceId) const;
public:
    TriangleMesh(){}
    TriangleMesh(std::string file);
    TriangleMesh(ParameterSurface const& bs, int m, int n);

    void loadFromObj (std::istream& in);
    void loadFromObj (std::string file);
    void writeToObj (std::ostream& os) const;
    // 设置完点面后，进行建树等初始化操作
    void init ();
    void buildKDTree ();
    void buildAABBTree ();
    void fixFaceNormal ();

    AxisBox getAABB() const override;
    Vector3f paramToGravity (Vector3f const& param) const;
    bool testRayBlocked(Ray const &ray, float tmin) const override;
    // param: (gravity.x, gravity.y, faceId)
    bool tryGetIntersectionPoint(Ray const &ray, float &t, Vector3f &param) const override;

    bool isOnSurface(Vector3f const &point) const override;
    int getFaceId(Vector3f const &point) const;
    Vector3f getNormalVector(Vector3f const &param) const override;
    bool isInside(Vector3f const &point) const override;

    friend std::ostream &operator<<(std::ostream &os, const TriangleMesh &mesh);
};


#endif //INC_2RAYTRACE_TRIANGLEMESH_H
