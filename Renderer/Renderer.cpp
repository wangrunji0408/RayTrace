//
// Created by 王润基 on 2017/4/15.
//

#include <chrono>
#include "Renderer.h"

Renderer::Renderer(shared_ptr<World> world, shared_ptr<Camera> camera) :
        world(world), camera(camera)
{
    h = camera->getHeight(), w = camera->getWidth();
    clear();
}

bool Renderer::render() {
    ++renderTimes;
    time_t t0 = time(0);
    if(enableParallel)
        mat.forEach([this](cv::Vec3f & p, const int id[])
                {
                    p += cv::Vec3f(renderPixel(id[0], id[1]));
                });
    else
        for(int i=0; i<h; ++i)
            for(int j=0; j<w; ++j)
                mat[i][j] += cv::Vec3f(renderPixel(i, j));
    auto t = time(0) - t0;
    totalTime += t;
    std::cerr << renderTimes << " Render End. Time = " << t << "s. Total time = " << totalTime << "s." << std::endl;
    return renderTimes == round;
}

void Renderer::recolor(cv::Mat3f &mat) {
    float maxValue = 0;
    mat.forEach([&](cv::Vec3f const& p, const int * position)
                {
                    maxValue = std::max(maxValue, p[0]);
                    maxValue = std::max(maxValue, p[1]);
                    maxValue = std::max(maxValue, p[2]);
                });
    mat.forEach([&](cv::Vec3f & p, const int * position)
                {
                    p[0] /= maxValue;
                    p[1] /= maxValue;
                    p[2] /= maxValue;
                });
}

cv::Mat Renderer::getResult() const {
    if(renderTimes == 0)
        return cv::Mat3b::zeros(w, h);
    cv::Mat3f mat0;
    cv::Mat3b mat1;
    cv::resize(mat, mat0, cv::Size(w, h));
    if(enableRecolor)
        recolor(mat0);
    mat0 *= 255.0f / renderTimes;
    mat0.convertTo(mat1, CV_8UC3);
    return mat1;
}

shared_ptr<Camera> Renderer::getCamera() const {
    return camera;
}

inline int64_t getUsTime()
{
    using namespace std::chrono;
    return duration_cast<microseconds>(
            high_resolution_clock::now().time_since_epoch()).count();
}

inline float calcS2 (Color sum, Color sum2, int n)
{
    auto d = (sum2 - sum * sum * (2 - 1.0f/n) / n) / n;
    return d.sum();
}

Color Renderer::renderPixel(int x, int y) const {
    srand((unsigned int) (getUsTime() ^ rand()));

    auto sum = Color::zero;
    auto sum2 = Color::zero;
    int t = 0;
    for(; t <= maxSampleTimes;) {
        ++t;
        auto ray = camera->getRandRay(x, y);
        auto color = renderRay(ray);
        sum += color;
        sum2 += color * color;
        if(t >= minSampleTimes && s2 > 0 && calcS2(sum, sum2, t) < s2)
            break;
    }
    return sum / t;
}

void Renderer::clear() {
    totalTime = 0;
    renderTimes = 0;
    if(super)
    {
        camera->setResolution(w * 2, h * 2);
        mat = cv::Mat::zeros(h*2, w*2, CV_32FC3);
    }
    else
    {
        camera->setResolution(w, h);
        mat = cv::Mat::zeros(h, w, CV_32FC3);
    }
}

int Renderer::getRenderTimes() const {
    return renderTimes;
}
