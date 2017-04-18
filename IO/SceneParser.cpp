//
// Created by 王润基 on 2017/4/15.
//

#include "SceneParser.h"
#include "../Light/LightSource/PointLight.h"
#include "../Light/LightSource/ParallelLight.h"
#include "../Material/Lambertain.h"
#include "../Material/Phong.h"
#include "../Material/BlinnPhong.h"
#include "../Material/ModifiedPhong.h"
#include "../Shapes/1D/BezierLine.h"
#include "../Shapes/2D/Plane.h"
#include "../Shapes/2D/Triangle.h"
#include "../Shapes/3D/Sphere.h"
#include "../Shapes/3D/AxisBox.h"
#include "../Renderer/RayTracer.h"
#include "../Renderer/LightProjection.h"
#include <fstream>

using namespace Json;

World *SceneParser::load(const char *filePath) {
    std::ifstream fin(filePath);
    fin >> json;
    fin.close();
    world = buildWorld(json["world"]);
    for(auto const& j: json["renderers"]) {
        auto renderer = buildRenderer(j);
        rendererDict[renderer->name] = renderer;
    }
    return world;
}

World *SceneParser::buildWorld(Json::Value const &json) {
    auto world = new World();
    for(auto const& j: json["materials"]) {
        auto material = buildMaterial(j);
        materialDict[material->name] = material;
    }
    for(auto const& j: json["objects"]) {
        auto object = buildObject(j);
        world->addObject(object);
    }
    for(auto const& j: json["cameras"]) {
        auto camera = buildCamera(j);
        world->addCamera(camera);
    }
    for(auto const& j: json["lights"]) {
        auto light = buildLight(j);
        world->addLight(light);
    }
    world->setEnvColor(parseVector3fCanBeSingle(json["env_color"]));
    world->name = json["name"].asString();
    return world;
}

Camera *SceneParser::buildCamera(Json::Value const &json) {
    Vector3f pos = parseVector3f(json["pos"]);
    Vector3f target = parseVector3f(json["target"]);
    Vector3f up = parseVector3f(json["up"]);
    int width = json["resolution"][0].asInt();
    int height = json["resolution"][1].asInt();
    float realw = json["realw"].asFloat();
    bool orthographic = json["orthographic"].asBool();
    auto camera = new Camera(pos, target, up, width, height, realw, orthographic);
    camera->name = json["name"].asString();
    return camera;
}

Vector3f SceneParser::parseVector3f(Json::Value const &json) {
    return Vector3f(json[0].asFloat(), json[1].asFloat(), json[2].asFloat());
}

Vector3f SceneParser::parseVector3fCanBeSingle(Json::Value const &json) {
    if(json.isArray())
        return parseVector3f(json);
    float x = json.asFloat();
    return Vector3f(x, x, x);
}

Ray SceneParser::parseRay(Json::Value const &json) {
    return Ray(parseVector3f(json[0]), parseVector3f(json[1]));
}

Object *SceneParser::buildObject(Json::Value const &json) {
    auto shape = buildShape(json["shape"]);
    auto material = json["material"].isString()?
                    materialDict[json["material"].asString()]:
                    buildMaterial(json["material"]);
    auto name = json["name"].asString();
    return new Object(shape, material, name);
}

LightSource *SceneParser::buildLight(Json::Value const &json) {
    if(json["type"] == "point")
    {
        auto pos = parseVector3f(json["pos"]);
        auto color = parseVector3fCanBeSingle(json["color"]);
        auto light = new PointLight(pos, color);
        light->name = json["name"].asString();
        return light;
    }
    else if(json["type"] == "parallel")
    {
        auto pos = parseVector3f(json["pos"]);
        Vector3f dir;
        if(!json["dir"].isNull())
            dir = parseVector3f(json["dir"]);
        else if(!json["target"].isNull())
            dir = parseVector3f(json["target"]) - pos;
        else
            throw std::invalid_argument("ParallelLight does not contain 'dir' or 'target'.");
        auto color = parseVector3f(json["color"]);
        auto light = new ParallelLight(Ray(pos, dir), color);
        light->name = json["name"].asString();
        return light;
    }
    throw std::invalid_argument("LightSource type wrong.");
}

Material *SceneParser::buildMaterial(Json::Value const &json) {
    Material* material = nullptr;
    if(json["type"] == "phong" || json["type"] == "blinn_phong" || json["type"] == "modified_phong")
    {
        auto diffuse = parseVector3fCanBeSingle(json["diffuse"]);
        auto specular = parseVector3fCanBeSingle(json["specular"]);
        auto shininess = json["shininess"].asFloat();
        if(json["type"] == "phong")
            material = new Phong(diffuse, specular, shininess);
        else if(json["type"] == "blinn_phong")
            material = new BlinnPhong(diffuse, specular, shininess);
        else if(json["type"] == "modified_phong")
            material = new ModifiedPhong(diffuse, specular, shininess);
    }
    else if(json["type"] == "lambert")
    {
        auto diffuse = parseVector3fCanBeSingle(json["diffuse"]);
        material = new Lambertain(diffuse);
    }
    else
        throw std::invalid_argument("Material type wrong: " + json["name"].asString());
    material->reflectiveness = parseVector3fCanBeSingle(json["reflectiveness"]);
    material->transparency = parseVector3fCanBeSingle(json["transparency"]);
    material->refractiveIndex = json.get("refractive_index", 1).asFloat();
    material->name = json["name"].asString();
    return material;
}

Shape *SceneParser::buildShape(Json::Value const &json) {
    Shape* shape = nullptr;
    if(json["type"] == "bezier_line")
    {
        std::vector<Vector3f> points;
        for(auto const& j: json["points"])
            points.push_back(parseVector3f(j));
        shape = new BezierLine(points);
    }
    else if(json["type"] == "plane")
    {
        auto point = parseVector3f(json["point"]);
        auto normal = parseVector3f(json["normal"]);
        shape = new Plane(Ray(point, normal));
    }
    else if(json["type"] == "triangle")
    {
        Vector3f point[3];
        for(int i=0; i<3; ++i)
            point[i] = parseVector3f(json["points"][i]);
        shape = new Triangle(point);
    }
    else if(json["type"] == "sphere")
    {
        auto o = parseVector3f(json["o"]);
        auto r = json["r"].asFloat();
        shape = new Sphere(o, r);
    }
    else if(json["type"] == "axis_box")
    {
        auto minp = parseVector3f(json["min"]);
        auto maxp = parseVector3f(json["max"]);
        shape = new AxisBox(minp, maxp);
    }
    else
        throw std::invalid_argument("Shape type wrong: " + json["name"].asString());
    shape->name = json["name"].asString();
    return shape;
}

Renderer *SceneParser::buildRenderer(Json::Value const &json) {
    Renderer* renderer = nullptr;
    Camera* camera = world->findCamera(json["camera"].asString());
    if(json["type"] == "light_projection")
    {
        renderer = new LightProjection(world, camera);
    }
    else if(json["type"] == "ray_tracer")
    {
        auto rt = new RayTracer(world, camera);
        rt->setMaxDepth(json.get("depth", 2).asInt());
        renderer = rt;
    }
    renderer->enableRecolor = json["recolor"].asBool();
    renderer->name = json["name"].asString();
    return renderer;
}
