#include <iostream>
#include <opencv2/opencv.hpp>

#include "Shapes/2D/Triangle.h"
using namespace std;

int main() {
    auto a = Vector3f(0, 0, 0);
    auto b = Vector3f(1, 0, 0);
    auto c = Vector3f(0, 1, 0);
    auto tri = Triangle(a, b, c);
    auto p = Vector3f(1, 1, 0);
    cout << tri.calcGravityCoordinate(p) << endl;
    return 0;
}