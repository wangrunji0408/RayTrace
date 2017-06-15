//
// Created by 王润基 on 2017/4/18.
//

#include "TriangleMesh.h"
#include "../2D/BezierSurface.h"
#include <fstream>

using std::string;

inline int parseInt (const char* str)
{
    int x = 0;
    sscanf(str, "%d", &x);
    return x;
}

void TriangleMesh::loadFromObj(std::istream &in) {
    vs.push_back(Point::zero);
    vns.push_back(Point::zero);
    vts.push_back(Point::zero);
    faces.push_back(TriFace());

    string type, line;
    Point p;
    TriFace face;

    while(!in.eof())
    {
        std::stringstream ss;
        std::getline(in, line);
        ss << line;
        ss >> type;
        if(type == "v")
        {
            ss >> p.x >> p.y >> p.z;
            vs.push_back(p);
        }
        else if(type == "vt")
        {
            p.z = 0;
            ss >> p.x >> p.y >> p.z;
            vts.push_back(p);
        }
        else if(type == "vn")
        {
            ss >> p.x >> p.y >> p.z;
            vns.push_back(p);
        }
        else if(type == "f")
        {
            for(int i=0; i<3; ++i)
            {
                string token; ss >> token;
                face.v[i] = parseInt(token.c_str());
                int p1 = (int)token.find_first_of('/');
                int p2 = (int)token.find_last_of('/');
                if(p1 == std::string::npos)
                    continue;
                face.vt[i] = parseInt(token.c_str() + p1 + 1);
                face.vn[i] = parseInt(token.c_str() + p2 + 1);
            }
            faces.push_back(face);
        }
    }
    init();
}

bool TriangleMesh::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    Vector3f point, normal;
    return tryGetIntersectionInfo(ray, t, point, normal);
}

bool TriangleMesh::tryGetIntersectionInfo(Ray const &ray, float &t, Vector3f &point, Vector3f &normal) const {
    int face = 0;

    // AABB
    shared_ptr<Shape> shape;
    aabbTree.tryGetIntersectionInfo(ray, t, shape);
    face = int(dynamic_cast<Triangle*>(shape.get()) - triangles.data());

    /*
    // KDTree
    if(fastIntersect(ray) == inf)
        return false;
    int faceCnt = 0;
    t = inf;
    for(auto const& pair: kdTree.getIntersectSpaces(ray))
    {
        if(pair.t > t + eps)
            continue;
        faceCnt += faceIdsInSpace[pair.point + 1].size();
//        std::cerr << faceIdsInSpace[point + 1].size() << " ";
        for (int faceId: faceIdsInSpace[pair.point + 1]) {
            auto triangle = toTriangle(faceId);
            float tt;
            bool exist = triangle.tryGetIntersectionPoint(ray, tt);
            if (exist && tt < t - eps)
                t = tt, face = faceId;
        }
    }
//    std::cerr << faceCnt << std::endl;
    */


    /*
    // force
    if(fastIntersect(ray) == inf)
        return false;
    t = inf;
    for(int i=1; i<faces.size(); ++i) // force
    {
        float tt;
        bool exist = triangles[i].tryGetIntersectionPoint(ray, tt);
        if (exist && tt < t - eps)
            t = tt, face = i;
    }
     */


    if(t == inf)    return false;
    point = ray.getEndPoint(t);
    if(normalInterpolation) {
        auto gravity = toTriangle(face).calcGravityCoordinate(point);
        normal = vns[faces[face].vn[0]] * gravity.x
                 + vns[faces[face].vn[1]] * gravity.y
                 + vns[faces[face].vn[2]] * gravity.z;
    }
    else
        normal = toTriangle(face).getNormalVectorOnSurface(point);
    return true;
}

bool TriangleMesh::isOnSurface(Vector3f const &point) const {
    return getFaceId(point) != 0;
}

Vector3f TriangleMesh::getNormalVectorOnSurface(Vector3f const &point) const {
    for(int i=1; i<faces.size(); ++i)
    {
        auto triangle = toTriangle(i);
        if(triangle.isOnSurface(point))
            return triangle.getNormalVectorOnSurface(point);
    }
    throw std::exception();
}

bool TriangleMesh::isInside(Vector3f const &point) const {
    throw std::exception();
}

Triangle TriangleMesh::toTriangle(int faceId) const {
    auto const& face = faces[faceId];
    return Triangle(vs[face.v[0]], vs[face.v[1]], vs[face.v[2]]);
}

int TriangleMesh::getFaceId(Vector3f const &point) const {
    for(int i=1; i<faces.size(); ++i)
    {
        auto triangle = toTriangle(i);
        if(triangle.isOnSurface(point))
            return i;
    }
    return 0;
}

void TriangleMesh::loadFromObj(std::string file) {
    std::ifstream fin(file);
    loadFromObj(fin);
    fin.close();
}

TriangleMesh::TriangleMesh(std::string file)
{
    loadFromObj(file);
}

void TriangleMesh::buildKDTree() {
    kdTree = KDTree(vs.data() + 1, (int)vs.size() - 1);
    faceIdsInSpace.resize(vs.size());
    for(int i=1; i<faces.size(); ++i)
    {
        int spaceId = calcSpaceId(i);
        faceIdsInSpace[spaceId].push_back(i);
    }
}

int TriangleMesh::calcSpaceId(int faceId) const {
    auto points = std::vector<int>{faces[faceId].v[0]-1, faces[faceId].v[1]-1, faces[faceId].v[2]-1};
    return kdTree.calcCommonRoot(points) + 1;
}

void TriangleMesh::fixFaceNormal() {
    for(int i=1; i<faces.size(); ++i)
    {
        auto& face = faces[i];
        Point const& p0 = vs[face.v[0]];
        Point const& p1 = vs[face.v[1]];
        Point const& p2 = vs[face.v[2]];
        Point const& n0 = vns[face.vn[0]];
        if((p1-p0).det(p2-p0).dot(n0) < -eps)
            face.swap(0, 2);
    }

}

AxisBox TriangleMesh::getAABB() const {
    return boundingBox;
}

void TriangleMesh::buildAABBTree() {
    triangles.clear(); triangles.reserve(faces.size());
    for(int i=0; i<faces.size(); ++i)
        triangles.push_back(toTriangle(i));
    std::vector<shared_ptr<Shape>> shapes(faces.size());
    for(int i=0; i<faces.size(); ++i)
        shapes[i] = shared_ptr<Shape>(&triangles[i]);
    aabbTree.build(shapes);
}

std::ostream &operator<<(std::ostream &os, const TriangleMesh &mesh) {
    os << "[TriangleMesh v: " << mesh.vs.size()-1 << " f: " << mesh.faces.size()-1 << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const TriangleMesh::TriFace &face) {
    os << "[TriFace\n";
    for(int i=0; i<3; ++i)
        os << i << ": v: " << face.v[i] << " vt: " << face.vt[i] << " vn: " << face.vn[i] << "\n";
    return os << "]\n";
}

bool TriangleMesh::testRayBlocked(Ray const &ray, float tmin) const {
    return aabbTree.testRayBlocked(ray, tmin);
}

inline int getPointId (int i, int j, int n)
{
    return i * (n+1) + j + 1;
}

TriangleMesh::TriangleMesh(BezierSurface const &bs, int m, int n) {
    vs.push_back(Point::zero);
    vns.push_back(Point::zero);
    vts.push_back(Point::zero);
    faces.push_back(TriFace());
    if(m < 1 || n < 1)
        throw std::invalid_argument("BezierSurface to TriangleMesh: m, n must >= 1.");

    // point
    float u = 0, v = 0;
    const float du = 1.0f / m, dv = 1.0f / n;
    for(int i=0; i<=m; ++i, u += du, v = 0)
        for (int j = 0; j <= n; ++j, v += dv) {
            vs.push_back(bs.getPoint(u, v));
            vns.push_back(bs.getNormalVector(u, v));
            vts.push_back(Point(u, v, 0));
        }

    // face
    for(int i=0; i<m; ++i)
        for(int j=0; j<n; ++j)
        {
            // 0--1
            // |  |
            // 2--3
            int id0 = getPointId(i, j, n);
            int id1 = getPointId(i, j+1, n);
            int id2 = getPointId(i+1, j, n);
            int id3 = getPointId(i+1, j+1, n);
            faces.push_back(TriFace(id0, id2, id1));
            faces.push_back(TriFace(id3, id1, id2));
        }
    init();
}

void TriangleMesh::init() {
    faces.resize(std::min((size_t)cutSize, faces.size()));
    fixFaceNormal();
    boundingBox = AxisBox(vs.data() + 1, vs.size() - 1);
//    buildKDTree();
    buildAABBTree();
}

bool TriangleMesh::tryGetIntersectionInfo(Ray const &ray, float &t, int &faceId, float &u, float &v) const {
    shared_ptr<Shape> shape;
    bool exist = aabbTree.tryGetIntersectionInfo(ray, t, shape);
    if(!exist)  return false;
    faceId = int(dynamic_cast<Triangle*>(shape.get()) - triangles.data());
    auto point = ray.getEndPoint(t);
    auto gravity = toTriangle(faceId).calcGravityCoordinate(point);
    if(faceId & 1)  // 左上片
    {
        u = gravity.z;
        v = gravity.y;
    }
    else    // 右下片
    {
        u = 1 - gravity.y;
        v = 1 - gravity.z;
    }
    return true;
}

void TriangleMesh::TriFace::swap(int i, int j) {
    std::swap(v[i], v[j]);
    std::swap(vn[i], vn[j]);
    std::swap(vt[i], vt[j]);
}

TriangleMesh::TriFace::TriFace(int i, int j, int k) {
    v[0] = vt[0] = vn[0] = i;
    v[1] = vt[1] = vn[1] = j;
    v[2] = vt[2] = vn[2] = k;
}
