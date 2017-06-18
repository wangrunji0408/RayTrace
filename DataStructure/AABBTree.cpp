//
// Created by 王润基 on 2017/4/30.
//

#include "AABBTree.h"
#include "../Shapes/3D/AxisBox.h"

void AABBTree::build(std::vector<shared_ptr<Shape> > const &shapes) {
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

bool AABBTree::tryGetIntersectionInfo(Ray const &ray, float &t, shared_ptr<Shape> &shape) const {
    t = inf, shape = nullptr;
    (*aabbCount)++, (*total)++;
    if(subtreeAABBs[0].fastIntersect(ray) == inf)
        return false;
    int shapeId = -1;
    updateIntersectionInfo(ray, t, shapeId, 0, n);
    shape = shapeId == -1? nullptr: shapes[shapeId];
//    if((*total & 0xFFFF) == 0)
//        std::cerr << *total << ' ' << (float)(*aabbCount)/(*total) << ' ' << (float)(*shapeCount)/(*total) << std::endl;
    return t != inf;
}

void AABBTree::updateIntersectionInfo(Ray const &ray, float &t, int &shape, int begin, int end) const {
    if(end - begin <= 0)
        return;
    if(shapeAABBs[order[begin]].fastIntersect(ray) < t)
    {
        (*shapeCount)++, (*aabbCount)++;
        auto nodeShape = shapes[order[begin]].get();
        float t1 = nodeShape->calcIntersect(ray);
        if(t1 < t)
            t = t1, shape = order[begin];
    }
    if(end - begin == 1)
        return;
    auto const& leftAABB = subtreeAABBs[begin+1];
    auto const& rightAABB = subtreeAABBs[rightBegin[begin]];
    *aabbCount += 2;
    float leftT = leftAABB.fastIntersect(ray);
    float rightT = rightAABB.fastIntersect(ray);
    if(leftT < rightT)
    {
        if(leftT < t) updateIntersectionInfo(ray, t, shape, begin + 1, rightBegin[begin]);
        if(rightT < t) updateIntersectionInfo(ray, t, shape, rightBegin[begin], end);
    }
    else
    {
        if(rightT < t) updateIntersectionInfo(ray, t, shape, rightBegin[begin], end);
        if(leftT < t) updateIntersectionInfo(ray, t, shape, begin + 1, rightBegin[begin]);
    }
}

bool AABBTree::testRayBlocked(Ray const &ray, float tmin, int begin, int end) const {
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
        return (leftT < tmin && testRayBlocked(ray, tmin, begin + 1, rightBegin[begin]))
               || (rightT < tmin && testRayBlocked(ray, tmin, rightBegin[begin], end));
    else
        return (rightT < tmin && testRayBlocked(ray, tmin, rightBegin[begin], end))
               || (leftT < tmin && testRayBlocked(ray, tmin, begin + 1, rightBegin[begin]));
}

void AABBTree::getAllPotential(Ray const &ray, std::vector<shared_ptr<Shape>> &vec, int begin, int end) const {
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

bool AABBTree::testRayBlocked(Ray const &ray, float tmin) const {
    return testRayBlocked(ray, tmin, 0, n);
}

std::vector<shared_ptr<Shape>> AABBTree::getAllPotential(Ray const &ray) const {
    auto vec = std::vector<shared_ptr<Shape>>();
    getAllPotential(ray, vec, 0, n);
    return vec;
}
