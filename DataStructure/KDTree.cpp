//
// Created by 王润基 on 2017/4/20.
//

#include "KDTree.h"

void KDTree::buildTree(KDTree::Iter begin, KDTree::Iter end, int d) {
    if(end - begin <= 1)
        return;
    std::stable_sort(begin, end, [&](int const& i, int const& j){return cmp(i,j,d);});
    Iter mid = getMid(begin, end);
    buildTree(begin, mid, nextd(d));
    buildTree(mid+1, end, nextd(d));
}

KDTree::KDTree(const Point *points, int n) {
    ps.assign(points, points + n);
    order.resize(n);
    for(int i=0; i<n; ++i)
        order[i] = i;
    buildTree(order.begin(), order.end(), 0);
}

std::string KDTree::locate(int id) const {
    if(id < 0 || id >= ps.size())
        throw std::invalid_argument("Id not in range.");
    std::string result;
    Iterc begin = order.begin(), end = order.end();
    for(int d=0; begin != end; d = nextd(d)) {
        auto mid = getMid(begin, end);
        if(*mid == id)  // hit
            return result;
        if(cmp(id, *mid, d)) // left
            end = mid, result.push_back('0');
        else    // right
            begin = mid+1, result.push_back('1');
    }
    throw std::invalid_argument("KDTree: Failed to locate.");
}

KDTree::Iter KDTree::getMid(const KDTree::Iter &begin, const KDTree::Iter &end) {
    return begin + (end - begin) / 2;
}

KDTree::Iterc KDTree::getMid(const KDTree::Iterc &begin, const KDTree::Iterc &end) {
    return begin + (end - begin) / 2;
}

int KDTree::nextd(int d) const {
    return d == K-1? 0: d+1;
}

void KDTree::print() const {
    int i = 0;
    for(auto const& id: order)
        std::cout << i++ << "\tNo." << id << '\t' << ps[id] << std::endl;
}

std::vector<KDTreeIntersect> KDTree::getIntersectSpaces(Ray const &ray, AxisBox const &globalBound) const {
    std::vector<KDTreeIntersect> result;
    result.reserve(ps.size());
    getIntersectSpaces(ray, globalBound, result, order.begin(), order.end(), 0);
    std::sort(result.begin(), result.end(), [](KDTreeIntersect const& a, KDTreeIntersect const& b)
    {
        return a.t < b.t;
    });
    return result;
}

void KDTree::getIntersectSpaces(Ray const &ray, AxisBox const& bound, std::vector<KDTreeIntersect> &result,
                                KDTree::Iterc begin, KDTree::Iterc end, int d) const {
    float t;
    if(begin == end || !bound.tryGetIntersectionPoint(ray, t))
        return;
    Iterc mid = getMid(begin, end);
    result.push_back(KDTreeIntersect{*mid, t});
    float midValue = ps[*mid].value(d);
    AxisBox leftBound = bound, rightBound = bound;
    leftBound.maxp.value(d) = midValue;
    rightBound.minp.value(d) = midValue;
    getIntersectSpaces(ray, leftBound, result, begin, mid, nextd(d));
    getIntersectSpaces(ray, rightBound, result, mid+1, end, nextd(d));
}

int KDTree::calcCommonRoot(std::vector<int> const &ids) const {
    Iterc begin = order.begin(), end = order.end(), mid;
    for(int d=0; begin != end; d = nextd(d)) {
        mid = getMid(begin, end);
        int cnt0 = 0;
        for(int id : ids)
            cnt0 += cmp(id, *mid, d);
        if(cnt0 == ids.size()) // left
            end = mid;
        else if(cnt0 == 0)    // right
            begin = mid+1;
        else
            break;
    }
    return *mid;
}

int KDTree::idAt(std::string path) const {
    Iterc begin = order.begin(), end = order.end();
    for(int i=0; begin != end; ++i) {
        auto mid = getMid(begin, end);
        if(i == path.size())  // hit
            return *mid;
        if(path[i] == '0') // left
            end = mid;
        else    // right
            begin = mid+1;
    }
    throw std::invalid_argument("KDTree: Failed to get id at: " + path);
}

bool KDTree::cmp(int i, int j, int d) const {
    if(ps[i].value(d) == ps[j].value(d))
        return i < j;
    return ps[i].value(d) < ps[j].value(d);
}
