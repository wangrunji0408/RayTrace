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
    boundingBox = AxisBox(vs.data() + 1, vs.size() - 1);
    buildKDTree();
}

bool TriangleMesh::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    Vector3f point, normal;
    return tryGetIntersectionInfo(ray, t, point, normal);
}

bool TriangleMesh::tryGetIntersectionInfo(Ray const &ray, float &t, Vector3f &point, Vector3f &normal) const {
    if(!tryIntersect(ray))
        return false;
    int faceCnt = 0;
    int face = 0;
    t = inf;
//    for(int i=1; i<faces.size(); ++i) // force
    for(auto const& pair: kdTree.getIntersectSpaces(ray)) // use KDTree
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
        auto& face = faces[i];
        Point const& p0 = vs[face.v[0]];
        Point const& p1 = vs[face.v[1]];
        Point const& p2 = vs[face.v[2]];
        Point const& n0 = vns[face.vn[0]];
        if((p1-p0).det(p2-p0).dot(n0) < -eps)
            face.swap(0, 2);
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
