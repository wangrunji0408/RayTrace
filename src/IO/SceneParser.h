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
    std::map<std::string, shared_ptr<ObjectMaterial>> materialDict;
    std::vector<shared_ptr<Renderer>> rendererList;
    shared_ptr<World> world;
    Json::Value json;
protected:
    static Vector3f parseVector3f (Json::Value const& json);
    static Vector3f parseVector3fCanBeSingle (Json::Value const& json);
    static Transform parseTransform (Json::Value const& json);
    static Ray parseRay (Json::Value const& json);
    unique_ptr<Object> buildObject(Json::Value const &json);
    static unique_ptr<Shape> buildShape(Json::Value const &json);
    static unique_ptr<Texture> buildTexture(Json::Value const &json);
    static unique_ptr<UVMap> buildUVMap(Json::Value const &json);
    shared_ptr<ObjectMaterial> buildMaterial(Json::Value const &json);
    static unique_ptr<LightSource> buildLight(Json::Value const &json);
    unique_ptr<World> buildWorld(Json::Value const &json);
    static unique_ptr<Camera> buildCamera(Json::Value const &json);
    unique_ptr<Renderer> buildRenderer(Json::Value const &json);
public:
    shared_ptr<World> load(const char *filePath);
    shared_ptr<Renderer> getRenderer () const;
};


#endif //INC_2RAYTRACE_SCENEPARSER_H
