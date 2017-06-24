//
// Created by wangrunji0408 on 2017/6/24.
//

#ifndef INC_2RAYTRACE_PHOTONMAPS_H
#define INC_2RAYTRACE_PHOTONMAPS_H


#include "BiPathTracer.h"
#include "../Shapes/3D/Sphere.h"

class ProgressivePhotonMaps: public BiPathTracer {
protected:
    float r0 = 1;
    float alpha = 0.8;
    IntersectInfo colls[1000][1000];
    cv::Mat1f radius2;
    cv::Mat1i countPhoton;
    AABBTree aabbTree;
    std::vector<Sphere> balls;
    std::vector<shared_ptr<IRayCastable>> shapes;
public:
    ProgressivePhotonMaps(shared_ptr<World> world, shared_ptr<Camera> camera);
    bool render() override;
protected:
    cv::Mat3f renderRay(int times, cv::Mat1i &cnt) const;
    void rebuildTree ();
    void next (cv::Mat1i const& cnt);
public:
    void clear() override;
};


#endif //INC_2RAYTRACE_PHOTONMAPS_H
