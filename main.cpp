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
#include "Renderer/PathTracer.h"

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
    auto renderer = parser.getRenderer();

    char str[100];
    string savePath = "../image/";
    sprintf(str, "render(%s)_%d", renderer->name.c_str(), (int)time(0));
    string fileName = str;

    cv::Mat mat;
    for(bool finish = false; !finish; )
    {
        finish = renderer->render();
        mat = renderer->getResult();

        sprintf(str, "%s%s.png", savePath.c_str(), fileName.c_str(), renderer->getRenderTimes());
        cv::imwrite(str, mat);
        cv::imshow("render", mat);
        cv::waitKey(1);
    }

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

cv::Mat3b imageMerge (vector<string> const& paths)
{
    cv::Mat3i rst;
    bool first = true;
    int totalSample = 0;
    for(auto const& path: paths)
    {
        auto image = cv::imread(path);
        image = cv::Mat3i(image);
        if(first)
        {
            rst = cv::Mat3i::zeros(image.size[0], image.size[1]);
            first = false;
        }
        if(image.size != rst.size)
            throw std::invalid_argument("Image size is different: " + path);

        int sample = 1;
        rst += image * sample;
        totalSample += sample;
    }
    rst /= totalSample;
    return cv::Mat3b(rst);
}

int main (int argc, char** argv) {
//    testKDTree();
    const char* filePath;
    if(argc >= 2 && string(argv[1]) == "merge")
    {
        vector<string> paths;
        for(int i=2; i<argc; ++i)
            paths.push_back(string(argv[i]));
        auto image = imageMerge(paths);
        char file[100];
        sprintf(file, "../image/merge_%d.png", (int)clock());
        cv::imwrite(file, image);
    }
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