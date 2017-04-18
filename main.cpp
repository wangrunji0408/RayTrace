#include <iostream>
#include <opencv2/opencv.hpp>

#include "Shapes/2D/Triangle.h"
#include "Shapes/1D/BezierLine.h"
#include "World/World.h"
#include "Shapes/3D/Sphere.h"
#include "Renderer/LightProjection.h"
#include "Material/Lambertain.h"
#include "Light/LightSource/PointLight.h"
#include "Material/Phong.h"
#include "IO/SceneParser.h"
#include "Renderer/RayTracer.h"

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

void test() {
    auto world = new World();
    auto m1 = new Lambertain(Color(1, 0, 0));
    auto m11 = new Phong(Color(0.2, 0, 0), Color(0.4, 0, 0), 1);
    auto m2 = new Lambertain(Color(0, 0.5, 0));
    auto triangle = new Object(new Triangle(Point(-5, 0, 0), Point(0, 5, 0), Point(-5, 5, 5)), m11, "triangle");
    auto sphere = new Object(new Sphere(Point(0, 0, 0), 1), m11, "sphere");
    auto plane = new Object(new Plane(Ray(Point(0, 0, -0.5f), Point(0, 0, 1))), m2, "plane");
    //auto pointLight = new ParallelLight(Ray(Point(5, -5, 5), Point(-1, 1, -1)), Color(10, 10, 10));
    auto pointLight = new PointLight(Point(5, -5, 5), Color(1, 1, 1));
    world->addObject(sphere);
    world->addObject(plane);
    world->addObject(triangle);
    world->addLight(pointLight);

    auto camera = new Camera(Point(0, -10, 5), Point(0, 0, 0), Point(0, 0, 1),
                             640, 480, 10, false);
    auto renderer = new LightProjection(world, camera);
    auto mat = renderer->render();
    cv::imshow("image", mat);
    cv::waitKey(0);
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