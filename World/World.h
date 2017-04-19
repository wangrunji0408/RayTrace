//
// Created by 王润基 on 2017/4/8.
//

#ifndef INC_2RAYTRACE_WORLD_H
#define INC_2RAYTRACE_WORLD_H

#include <vector>
#include "../Light/Light.h"
#include "../Object/Object.h"
#include "../Light/LightSource/LightSource.h"
#include "../Object/IntersectResult.h"
#include "../Camera/Camera.h"

class World {
public:
    string name = "world";
private:
    Color envColor = Color::zero;
    std::vector<Object*> objects;
    std::vector<LightSource*> lights;
    std::vector<LightSource*> enabledLights;
    std::vector<Camera*> cameras;
public:
    const std::vector<LightSource *> &getLights() const;
    void addObject (Object* obj);
    void removeObject (Object* obj);
    void addLight (LightSource* light);
    void removeLight (LightSource* light);
    void addCamera (Camera* camera);
    void removeCamera  (Camera* camera);
    Camera* findCamera (std::string name) const;

    const Color &getEnvColor() const;
    void setEnvColor(const Color &envColor);

    IntersectResult tryGetFirstIntersectionPoint (Ray const& ray) const;
    bool testLightBlocked(Light const& light) const;

    static Vector3f calcRefractiveDir (Vector3f const &inDir, Vector3f const &normalDir, float indexInside, float indexOutside);
    static Vector3f calcReflectiveDir (Vector3f const &inDir, Vector3f const &normalDir);
};


#endif //INC_2RAYTRACE_WORLD_H
