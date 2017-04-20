//
// Created by 王润基 on 2017/4/18.
//

#include "TriangleMesh.h"
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
    faces.resize(std::min((size_t)cutSize, faces.size()));
    fixFaceNormal();
    buildBound();
    buildKDTree();
}

bool TriangleMesh::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    if(!tryIntersect(ray))
        return false;

    t = inf;
    float tt;
//    for(int i=1; i<faces.size(); ++i) // force
    for(int point: kdTree.getIntersectSpaces(ray, boundingBox)) // use KDTree
        for(int faceId: faceIdsInSpace[point+1])
    {
        auto triangle = toTriangle(faceId);
        bool exist = triangle.tryGetIntersectionPoint(ray, tt);
        if(exist) t = std::min(tt, t);
    }
    return t != inf;
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

void TriangleMesh::buildBound() {
    boundingBox.minp = Point(inf, inf, inf);
    boundingBox.maxp = -Point(inf, inf, inf);
    for(int i=1; i<vs.size(); ++i){
        Point const& p = vs[i];
        updateMin(boundingBox.minp.x, p.x);
        updateMin(boundingBox.minp.y, p.y);
        updateMin(boundingBox.minp.z, p.z);
        updateMax(boundingBox.maxp.x, p.x);
        updateMax(boundingBox.maxp.y, p.y);
        updateMax(boundingBox.maxp.z, p.z);
    }
}

bool TriangleMesh::tryIntersect(Ray const &ray) const {
    float t;
    return boundingBox.tryGetIntersectionPoint(ray, t);
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
        Point const& p0 = vs[faces[i].v[0]];
        Point const& p1 = vs[faces[i].v[1]];
        Point const& p2 = vs[faces[i].v[2]];
        Point const& n0 = vns[faces[i].vn[0]];
        if((p1-p0).det(p2-p0).dot(n0) < -eps)
            faces[i].swap(0, 2);
    }

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

void TriangleMesh::TriFace::swap(int i, int j) {
    std::swap(v[i], v[j]);
    std::swap(vn[i], vn[j]);
    std::swap(vt[i], vt[j]);
}
