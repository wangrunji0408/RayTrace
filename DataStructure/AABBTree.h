//
// Created by 王润基 on 2017/4/30.
//

#ifndef INC_2RAYTRACE_AABBTREE_H
#define INC_2RAYTRACE_AABBTREE_H


#include <vector>
#include "../Shapes/Shape.h"

using std::vector;

class AABBTree {
private:
    static const int K = 3;
    int n;
    std::vector<AxisBox> subtreeAABBs;
    std::vector<int> order;
    std::vector<AxisBox> shapeAABBs;
    std::vector<shared_ptr<Shape>> shapes;
    std::vector<int> rightBegin;
    // used for building
    std::vector<AxisBox> leftAABBs, rightAABBs;
    std::vector<float> surfaceAreas;
public:
    // debug
    int* shapeCount = new int(0);
    int* aabbCount = new int(0);
    int* total = new int(0);
private:
    void buildTree (int begin, int end, int d);
    void updateIntersectionInfo(Ray const &ray, float &t, int &shape, int begin, int end) const;
    int devide (int begin, int end, int d); // 返回右子树begin位置，若为begin则不分割
    int nextd (int d) const;
public:
    void build (std::vector<shared_ptr<Shape> > const& shapes);
    bool tryGetIntersectionInfo (Ray const& ray, float &t, shared_ptr<Shape> &shape) const;
};


#endif //INC_2RAYTRACE_AABBTREE_H
