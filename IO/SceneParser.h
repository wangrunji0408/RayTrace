//
// Created by 王润基 on 2017/4/15.
//

#ifndef INC_2RAYTRACE_SCENEPARSER_H
#define INC_2RAYTRACE_SCENEPARSER_H

#include "json/json.h"
#include "../World/World.h"
#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"

class SceneParser {
public:
    std::map<std::string, ObjectMaterial*> materialDict;
    std::map<std::string, Renderer*> rendererDict;
    World* world;
    Json::Value json;
protected:
    static Vector3f parseVector3f (Json::Value const& json);
    static Vector3f parseVector3fCanBeSingle (Json::Value const& json);
    static Ray parseRay (Json::Value const& json);
    Object* buildObject (Json::Value const& json);
    static Shape* buildShape (Json::Value const& json);
    static Texture* buildTexture (Json::Value const& json);
    static UVMap* buildUVMap (Json::Value const& json);
    ObjectMaterial * buildMaterial(Json::Value const &json);
    static LightSource* buildLight (Json::Value const& json);
    World* buildWorld (Json::Value const& json);
    static Camera* buildCamera (Json::Value const& json);
    Renderer* buildRenderer (Json::Value const& json);
public:
    World* load (const char* filePath);
};


#endif //INC_2RAYTRACE_SCENEPARSER_H
