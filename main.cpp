#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

#include "Shapes/2D/Triangle.h"
#include "Shapes/1D/BezierCurve.h"
#include "World/World.h"
#include "Renderer/LightProjection.h"
#include "Light/LightSource/PointLight.h"
#include "IO/SceneParser.h"
#include "Renderer/RayTracer.h"
#include "Shapes/3D/TriangleMesh.h"
#include "Shapes/2D/BezierSurface.h"

using namespace std;

void testBezier() {
    vector<Vector3f> points = {Vector3f(0, 0, 0),
                               Vector3f(1, 2, 3),
                               Vector3f(0, 1, 2)};
    BezierCurve bezier = BezierCurve(points);
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

void testKDTree ()
{
    Point ps[] = {Point(1, 2, 0), Point(3, 2, 0), Point(4, 1, 0), Point(3, 1, 0),
                  Point(5, 3, 0), Point(1, 4, 0), Point(2, 2, 0), Point(2, 3, 0)};
    auto kd = KDTree(ps, 8);
    kd.print();
    cerr << kd.locate(2) << endl;
    cerr << "Common Root of {2, 3, 5}: " << kd.calcCommonRoot({2,3,5}) << endl;
    cerr << "Common Root of {2, 3}: " << kd.calcCommonRoot({2,3}) << endl;
}

inline cv::Point toPoint (Vector3i const& point)
{
    return cv::Point(point.y, point.x);
}

void testLoadFromFile (const char* filePath)
{
    auto parser = SceneParser();
    auto world = parser.load(filePath);
    auto renderer = parser.rendererDict.begin()->second;
    auto mat = renderer->render8U3C();

    auto rr = dynamic_pointer_cast<RayTracer>(renderer);
    auto camera = renderer->getCamera();
    if(rr != nullptr && !parser.json["show_light"].isNull()) {
        int x = parser.json["show_light"][0].asInt();
        int y = parser.json["show_light"][1].asInt();
        for (auto l: rr->renderPixelGetLights(x, y)) {
            auto p1 = toPoint(camera->getPos(l.begin));
            auto p2 = toPoint(camera->getPos(l.end));
            cv::arrowedLine(mat, p1, p2, cv::Scalar(255, 255, 255), 6);
            cv::arrowedLine(mat, p1, p2, cv::Scalar(0,0,0), 4);
            cv::arrowedLine(mat, p1, p2, l.color * 255, 2);
            cerr << l << endl;
        }
    }

    cerr << "Intersect Count:\n" << "Triangle: " << Triangle::intersectCount
                                 << "\nAxisBox: " << AxisBox::intersectCount << endl;
    char file[100];
    sprintf(file, "./image/render(%s)_%d.png", renderer->name.c_str(), (int)clock());
    cv::imwrite(file, mat);
    cv::imshow("image", mat);
    cv::waitKey(0);
}

void testParameterSurfaceToMesh (const char* filePath)
{
    string name = "spindle";
    string objPath = "../Scenes/" + name + ".obj";
    std::ofstream fout(objPath);

    auto parser = SceneParser();
    auto world = parser.load(filePath);
    auto shape = world->findObject(name)->getShape();
    auto bs = dynamic_pointer_cast<ParameterSurface>(shape);
    if(bs == nullptr)
        throw std::invalid_argument("Object shape type is not ParameterSurface: " + name);
    bs->mesh.writeToObj(fout);
}

int main (int argc, char** argv) {
//    testKDTree();
    const char* filePath;
    if(argc == 1)
        filePath = "scene.json";
    else if(argc == 2)
        filePath = argv[1];
    else
        return 0;
    testLoadFromFile(filePath);
//    testParameterSurfaceToMesh(filePath);
    return 0;
}