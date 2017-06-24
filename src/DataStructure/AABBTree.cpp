//
// Created by 王润基 on 2017/4/30.
//

#include "AABBTree.h"
#include "../Shapes/3D/AxisBox.h"

void AABBTree::build(std::vector<shared_ptr<IRayCastable> > const &shapes) {
    size_t n = shapes.size();
    this->n = (int)n;
    this->shapes = shapes;
    rightBegin.assign(n, 0);
    subtreeAABBs.assign(n, AxisBox());
    shapeAABBs.clear(); shapeAABBs.reserve(n);
    for(int i=0; i<n; ++i)
        shapeAABBs.push_back(shapes[i]->getAABB());
    order.clear();
    order.reserve(n);
    for(int i=0; i<n; ++i)
        order[i] = i;

    leftAABBs.assign(n, AxisBox());
    rightAABBs.assign(n, AxisBox());
    surfaceAreas.assign(n, 0);
    buildTree(0, this->n, 0);
    leftAABBs.clear();
    rightAABBs.clear();
    surfaceAreas.clear();
}

int AABBTree::nextd(int d) const {
    return d == K-1? 0: d+1;
}

void AABBTree::buildTree(int begin, int end, int d) {
    if(end - begin <= 0)    return;
    int mid = devide(begin, end, d);
    buildTree(begin+1, mid, nextd(d));
    buildTree(mid, end, nextd(d));
}

int AABBTree::devide(int begin, int end, int d) {
    std::sort(order.begin() + begin, order.begin() + end,
              [&](int const& i, int const& j) -> bool {
                  return shapeAABBs[i].getMidPoint().value(d) < shapeAABBs[j].getMidPoint().value(d);
              });
    leftAABBs[begin] = shapeAABBs[order[begin]];
    for(int i = begin+1; i<end; ++i)
        leftAABBs[i] = leftAABBs[i-1].merge(shapeAABBs[order[i]]);
    rightAABBs[end-1] = shapeAABBs[order[end-1]];
    for(int i = end-2; i>=begin; --i)
        rightAABBs[i] = rightAABBs[i+1].merge(shapeAABBs[order[i]]);
    for(int i = begin; i<end; ++i)
        surfaceAreas[i] = (i == begin? 0: leftAABBs[i-1].getSurfaceArea() * (i - begin))
                          + (i == end-1? 0: rightAABBs[i+1].getSurfaceArea() * (end-1 - i))
                            + shapeAABBs[order[i]].getSurfaceArea();
    int mid = int(std::min_element(surfaceAreas.begin() + begin, surfaceAreas.begin() + end) - surfaceAreas.begin());
    std::swap(order[begin], order[mid]);
    rightBegin[begin] = mid + 1;
    subtreeAABBs[begin] = leftAABBs[end - 1];
    return mid + 1;
}

void AABBTree::intersect(IntersectInfo &info, int begin, int end) const {
    if(end - begin <= 0)
        return;
    if(shapeAABBs[order[begin]].fastIntersect(info.ray) < info.t)
    {
        (*shapeCount)++, (*aabbCount)++;
        auto nodeShape = shapes[order[begin]].get();
        auto info1 = info;
        nodeShape->intersect(info1);
        if(info1.success && info1.t < info.t)
            info = info1;
    }
    if(end - begin == 1)
        return;
    auto const& leftAABB = subtreeAABBs[begin+1];
    auto const& rightAABB = subtreeAABBs[rightBegin[begin]];
    *aabbCount += 2;
    float leftT = leftAABB.fastIntersect(info.ray);
    float rightT = rightAABB.fastIntersect(info.ray);
    if(leftT < rightT)
    {
        if(leftT < info.t) intersect(info, begin + 1, rightBegin[begin]);
        if(rightT < info.t) intersect(info, rightBegin[begin], end);
    }
    else
    {
        if(rightT < info.t) intersect(info, rightBegin[begin], end);
        if(leftT < info.t) intersect(info, begin + 1, rightBegin[begin]);
    }
}

bool AABBTree::testRayBlocked0(Ray const &ray, float tmin, int begin, int end) const {
    if(end - begin <= 0)
        return false;
    if(shapeAABBs[order[begin]].fastIntersect(ray) < tmin)
    {
        auto const& nodeShape = shapes[order[begin]];
        if(nodeShape->testRayBlocked(ray, tmin))
            return true;
    }
    if(end - begin == 1)
        return false;
    auto const& leftAABB = subtreeAABBs[begin+1];
    auto const& rightAABB = subtreeAABBs[rightBegin[begin]];
    float leftT = leftAABB.fastIntersect(ray);
    float rightT = rightAABB.fastIntersect(ray);
    if(leftT < rightT)
        return (leftT < tmin && testRayBlocked0(ray, tmin, begin + 1, rightBegin[begin]))
               || (rightT < tmin && testRayBlocked0(ray, tmin, rightBegin[begin], end));
    else
        return (rightT < tmin && testRayBlocked0(ray, tmin, rightBegin[begin], end))
               || (leftT < tmin && testRayBlocked0(ray, tmin, begin + 1, rightBegin[begin]));
}

void AABBTree::getAllPotential(Ray const &ray, std::vector<shared_ptr<IRayCastable>> &vec, int begin, int end) const {
    if(end - begin <= 0)
        return;
    if(subtreeAABBs[begin].fastIntersect(ray) == inf)
        return;
    if(shapeAABBs[order[begin]].fastIntersect(ray) != inf)
        vec.push_back(shapes[order[begin]]);
    if(end - begin == 1)
        return;
    getAllPotential(ray, vec, begin + 1, rightBegin[begin]);
    getAllPotential(ray, vec, rightBegin[begin], end);
}

std::vector<shared_ptr<IRayCastable>> AABBTree::getAllPotential(Ray const &ray) const {
    auto vec = std::vector<shared_ptr<IRayCastable>>();
    getAllPotential(ray, vec, 0, n);
    return vec;
}

void AABBTree::intersect(IntersectInfo &info) const {
    if(n == 0)
        throw std::runtime_error("AABBTree is empty.");
    (*aabbCount)++, (*total)++;
    if(subtreeAABBs[0].fastIntersect(info.ray) == inf) {
        info.success = false;
        return;
    }
    if(info.testBlockT != 0) {
        info.success = testRayBlocked0(info.ray, info.testBlockT, 0, n);
        return;
    }
    intersect(info, 0, n);
}
