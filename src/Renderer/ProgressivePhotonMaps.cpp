//
// Created by wangrunji0408 on 2017/6/24.
//

#include <omp.h>
#include "ProgressivePhotonMaps.h"

ProgressivePhotonMaps::ProgressivePhotonMaps(shared_ptr<World> world, shared_ptr<Camera> camera) : BiPathTracer(world, camera) {

}

cv::Mat3f ProgressivePhotonMaps::renderRay(int times, cv::Mat1i &cnt) const {
    srand((unsigned int) (getUsTime() ^ rand()));
    std::vector<IntersectInfo> forwardPath;
    LightSource *lightSource = nullptr;
    cv::Mat3f mat1 = cv::Mat3f::zeros(mat.size[0], mat.size[1]);
    cnt = cv::Mat1i::zeros(mat.size[0], mat.size[1]);
    for(int i=0; i<times; ++i) {
        sampleForwardPath(forwardPath, lightSource);

        for (auto const &info: forwardPath) {
            auto point = info.getPoint();
            auto shapes = aabbTree.getAllPotential(point);
            for (auto const &ptr: shapes) {
                auto sphere = (Sphere *) ptr.get();
                int k = (int) (sphere - balls.data());
                int i = k / camera->getWidth();
                int j = k % camera->getWidth();
                if ((point - sphere->getO()).len2() >= radius2.at<float>(i, j))
//                if ((point - sphere->getO()).len() >= r)
                    continue;
                auto const &pinfo = colls[i][j];
//            if(!pinfo.success) continue;
                if (pinfo.object != info.object) continue;
                auto const &l = -info.ray.getUnitDir();
                auto const &v = -pinfo.ray.getUnitDir();
                auto const &n = pinfo.normal;
                auto material = pinfo.getMaterial();
                auto color = info.weight * pinfo.weight * material.calcBRDF(l, v, n);
                mat1.at<cv::Vec3f>(i, j) += cv::Vec3f(color);
                cnt.at<int>(i, j) += 1;

            }
        }
    }
    cv::Mat1f temp = radius2 * ((float)M_PI * times);
    mat1.forEach([&](cv::Vec3f &pixel, const int *pos){
        pixel /= temp.at<float>(pos);
    });
    return mat1;
}

bool ProgressivePhotonMaps::render() {
    ++renderTimes;
    time_t t0 = time(0);
    const int N = 4;
    cv::Mat mat1[N];
    cv::Mat1i cnt1[N];

#pragma omp parallel for
    for(int i=0; i<N; ++i)
        mat1[i] = renderRay((int)1e5, cnt1[i]);
    // accumlate
    for(int i=1; i<N; ++i)
        mat1[0] += mat1[i];
    mat += mat1[0] / N;
    for(int i=1; i<N; ++i)
        cnt1[0] += cnt1[i];
    next(cnt1[0]);
    auto t = time(0) - t0;
    totalTime += t;
    std::cerr << renderTimes << " Render End. Time = " << t << "s. Total time = " << totalTime << "s." << std::endl;
    return renderTimes == round;
}

void ProgressivePhotonMaps::clear() {
    Renderer::clear();
    int h = camera->getHeight();
    int w = camera->getWidth();
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
        {
            auto ray = camera->getRandRay(i, j);
            auto weight = Vector3f(1);
            Vector3f v;
            while (true) {
                auto info = world->tryGetFirstIntersectionPoint(ray);
                if(!info.success)   break;
                auto material = info.getMaterial();
                auto point = info.getPoint();
                if (material.reflection != Color::zero) {
                    v = World::calcReflectiveDir(-info.ray.getUnitDir(), info.normal);
                    weight *= material.reflection;
                } else if (material.refraction != Color::zero) {
                    v = material.calcRefractiveDir(-info.ray.getUnitDir(), info.normal);
                    weight *= material.refraction;
                } else {
                    info.weight = weight;
                    colls[i][j] = info;
                    break;
                }
                ray = Ray(point + v * 1e-4, v);
            }
        }
    radius2 = cv::Mat1f::ones(h, w) * r0;
    countPhoton = cv::Mat1i::ones(h, w); // 防止除零错误

    shapes.resize((unsigned) (h * w));
    balls.resize((unsigned) (h * w));
    for (int i = 0, k = 0; i < h; ++i)
        for (int j = 0; j < w; ++j, ++k)
            shapes[k] = unique_ptr<IRayCastable>(&balls[k]);
    rebuildTree();
}

void ProgressivePhotonMaps::rebuildTree() {
    int h = camera->getHeight();
    int w = camera->getWidth();
    for (int i = 0, k = 0; i < h; ++i)
        for (int j = 0; j < w; ++j, ++k)
            balls[k] = colls[i][j].success?
                       Sphere(colls[i][j].getPoint(), sqrtf(radius2.at<float>(i, j))):
                       Sphere(Vector3f(inf), 0);
    aabbTree = AABBTree();
    aabbTree.build(shapes);
}

void ProgressivePhotonMaps::next(cv::Mat1i const& cnt) {
    cv::Mat1i nextCount = countPhoton + cv::Mat1i(cnt * alpha);
    cv::Mat1f temp = cv::Mat1f(nextCount) / cv::Mat1f(countPhoton + cnt);
    radius2.forEach([&](float &pixel, const int *pos){
        pixel *= temp.at<float>(pos);
    });
    countPhoton = nextCount;
}


