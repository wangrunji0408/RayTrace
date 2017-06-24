//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_WORLD_H
#define INC_2RAYTRACE_WORLD_H

#include <vector>
#include "../Light/Light.h"
#include "../Object/Object.h"
#include "../Light/LightSource/LightSource.h"
#include "../Geometry/IntersectInfo.h"
#include "../Camera/Camera.h"
#include "../DataStructure/AABBTree.h"

class World {
public:
    string name = "world";
private:
    Color envColor = Color::zero;
    std::vector<shared_ptr<Object>> objects;
    std::vector<shared_ptr<LightSource>> lights;
    std::vector<shared_ptr<LightSource>> enabledLights;
    std::vector<shared_ptr<Camera>> cameras;
    AABBTree aabbTree;
public:
    const std::vector<shared_ptr<LightSource>> &getLights() const;
    void addObject(shared_ptr<Object> obj);
    void removeObject(shared_ptr<Object> obj);
    void addLight(shared_ptr<LightSource> light);
    void removeLight(shared_ptr<LightSource> light);
    void addCamera(shared_ptr<Camera> camera);
    void removeCamera(shared_ptr<Camera> camera);
    shared_ptr<Camera> findCamera(std::string name) const;
    shared_ptr<Object> findObject(std::string name) const;

    void makeAABBTree ();
    const Color &getEnvColor() const;
    void setEnvColor(const Color &envColor);
    IntersectInfo tryGetFirstIntersectionPoint (Ray const& ray) const;
    bool testLightBlocked(Light const& light) const;
    bool testLightBlocked(Point const& p1, Point const& p2) const;
    bool testLightBlocked(Ray const& ray, float t) const;

    static Vector3f calcRefractiveDir (Vector3f const &inDir, Vector3f const &normalDir, float indexInside, float indexOutside);
    static Vector3f calcReflectiveDir (Vector3f const &inDir, Vector3f const &normalDir);
    static float calcG (Ray const& face1, Ray const& face2);
};


#endif //INC_2RAYTRACE_WORLD_H
