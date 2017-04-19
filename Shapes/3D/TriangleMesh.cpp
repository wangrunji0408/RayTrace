//
// Created by 王润基 on 2017/4/18.
//

#include "TriangleMesh.h"
#include <sstream>
#include <fstream>

using std::string;

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
            ss >> face.v[0] >> face.v[1] >> face.v[2];
            // TODO 支持f的其他格式
            faces.push_back(face);
        }
    }

    buildSphere();
}

bool TriangleMesh::tryGetIntersectionPoint(Ray const &ray, float &t) const {
    if(!tryIntersect(ray))
        return false;
    t = inf;
    float tt;
    for(int i=1; i<faces.size(); ++i)
    {
        auto triangle = toTriangle(i);
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

void TriangleMesh::buildSphere() {
    sphere = Sphere(Point::zero, 2);
}

bool TriangleMesh::tryIntersect(Ray const &ray) const {
    float t;
    return sphere.tryGetIntersectionPoint(ray, t);
}

std::ostream &operator<<(std::ostream &os, const TriangleMesh &mesh) {
    os << "[TriangleMesh v: " << mesh.vs.size()-1 << " f: " << mesh.faces.size()-1 << "]";
    return os;
}
