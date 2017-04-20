//
// Created by 王润基 on 2017/4/20.
//

#ifndef INC_2RAYTRACE_KDTREE_H
#define INC_2RAYTRACE_KDTREE_H


#include <vector>
#include "../Geometry/Geometry.h"
#include "../Shapes/3D/AxisBox.h"

struct KDTreeIntersect
{
    int point;
    float t;
};

class KDTree {
    static const int K = 3;
    std::vector<Point> ps;  //0 ... n-1
    std::vector<int> order;
    typedef std::vector<int>::iterator Iter;
    typedef std::vector<int>::const_iterator Iterc;
private:
    void buildTree (Iter begin, Iter end, int d);
    inline static Iter getMid (Iter const& begin, Iter const& end);
    inline static Iterc getMid (Iterc const& begin, Iterc const& end);
    inline int nextd (int d) const;
    bool cmp (int i, int j, int d) const;
    void getIntersectSpaces (Ray const& ray, AxisBox const& bound, std::vector<KDTreeIntersect> &result, Iterc begin, Iterc end, int d) const;
public:
    KDTree () {}
    KDTree (const Point* points, int n);
    std::string locate(int id) const;
    int idAt (std::string path) const;
    // 每个顶点对应被其划分的长方体空间，返回所有与射线相交的长方体对应的点
    std::vector<KDTreeIntersect> getIntersectSpaces (Ray const& ray, AxisBox const& globalBound) const;
    int calcCommonRoot(std::vector<int> const &ids) const;
    void print () const;
};


#endif //INC_2RAYTRACE_KDTREE_H
