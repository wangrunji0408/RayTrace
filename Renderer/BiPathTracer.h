//
// Created by 王润基 on 2017/6/21.
//

#ifndef INC_2RAYTRACE_BIPATHTRACER_H
#define INC_2RAYTRACE_BIPATHTRACER_H


#include "PathTracer.h"

class BiPathTracer: public PathTracer {
public:
    BiPathTracer(shared_ptr<World> world, shared_ptr<Camera> camera);
protected:
    void sampleForwardPath(std::vector<IntersectInfo> &forwardPath, LightSource *&lightSource) const;
    void sampleBackwardPath(Ray const& ray, std::vector<IntersectInfo>& backwardPath) const;
    Color renderRay(Ray const &ray, int depth, Color weight) const override;
};


#endif //INC_2RAYTRACE_BIPATHTRACER_H
