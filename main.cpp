#include <iostream>
#include <opencv2/opencv.hpp>

#include "Shapes/2D/Triangle.h"
#include "Shapes/1D/BezierLine.h"
#include "World/World.h"
#include "Renderer/LightProjection.h"
#include "Light/LightSource/PointLight.h"
#include "IO/SceneParser.h"
#include "Renderer/RayTracer.h"
#include "Shapes/3D/TriangleMesh.h"

using namespace std;

void testBezier() {
    vector<Vector3f> points = {Vector3f(0, 0, 0),
                               Vector3f(1, 2, 3),
                               Vector3f(0, 1, 2)};
    BezierLine bezier = BezierLine(points);
    cout << bezier.getPoint(0.25f) << endl;
    cout << bezier.getPoint(0.5f) << endl;
    cout << bezier.getPoint(0.75f) << endl;
    cout << bezier.getPoint(1.00f) << endl;
}

void testTriangleGravity() {
    auto a = Vector3f(0, 0, 0);
    auto b = Vector3f(1, 0, 0);
    auto c = Vector3f(0, 1, 0);
    auto tri = Triangle(a, b, c);
    auto p = Vector3f(1, 1, 0);
    cout << tri.calcGravityCoordinate(p) << endl;
}

cv::Scalar toScalar (Color const& color)
{
    return cv::Scalar(color.x, color.y, color.z) * 255;
}

cv::Point toPoint (Vector3i const& point)
{
    return cv::Point(point.y, point.x);
}

void testLoadFromFile (const char* filePath)
{
    auto parser = SceneParser();
    World* world = parser.load(filePath);
    auto renderer = parser.rendererDict.begin()->second;
    auto mat = renderer->render8U3C();

    auto rr = dynamic_cast<RayTracer*>(renderer);
    auto camera = renderer->getCamera();
    if(rr != nullptr && !parser.json["show_light"].isNull()) {
        int x = parser.json["show_light"][0].asInt();
        int y = parser.json["show_light"][1].asInt();
        for (auto l: rr->renderPixelGetLights(x, y)) {
            auto p1 = toPoint(camera->getPos(l.begin));
            auto p2 = toPoint(camera->getPos(l.end));
            cv::arrowedLine(mat, p1, p2, cv::Scalar(255, 255, 255), 6);
            cv::arrowedLine(mat, p1, p2, cv::Scalar(0,0,0), 4);
            cv::arrowedLine(mat, p1, p2, toScalar(l.color), 2);
            cerr << l << endl;
        }
    }

    char file[100];
    sprintf(file, "./image/render(%s)_%d.png", renderer->name.c_str(), (int)clock());
    cv::imwrite(file, mat);
    cv::imshow("image", mat);
    cv::waitKey(0);
}

int main (int argc, char** argv) {
    const char* filePath;
    if(argc == 1)
        filePath = "scene.json";
    else if(argc == 2)
        filePath = argv[1];
    else
        return 0;
    testLoadFromFile(filePath);
    return 0;
}