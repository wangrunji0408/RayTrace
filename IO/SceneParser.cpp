//
// Created by 王润基 on 2017/4/15.
//

#include "SceneParser.h"
#include "../Light/LightSource/PointLight.h"
#include "../Light/LightSource/ParallelLight.h"
#include "../Shapes/1D/BezierLine.h"
#include "../Shapes/2D/Plane.h"
#include "../Shapes/2D/Triangle.h"
#include "../Shapes/3D/Sphere.h"
#include "../Shapes/3D/AxisBox.h"
#include "../Renderer/RayTracer.h"
#include "../Renderer/LightProjection.h"
#include "../Shapes/3D/TriangleMesh.h"
#include "../Light/LightSource/SpotLight.h"
#include "../Renderer/PathTracer.h"
#include "../Texture/ImageTexture.h"
#include "../Texture/GridTexture.h"
#include "../Texture/ConstTexture.h"
#include "../Texture/UVMaps/SphereMap.h"
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
    auto material = buildMaterial(json["material"]);
    auto uvMap = buildUVMap(json["uvmap"]);
    auto name = json["name"].asString();
    return new Object(shape, material, uvMap, name);
}

LightSource *SceneParser::buildLight(Json::Value const &json) {
    LightSource* light = nullptr;
    if(json["type"] == "point")
    {
        auto pos = parseVector3f(json["pos"]);
        auto color = parseVector3fCanBeSingle(json["color"]);
        light = new PointLight(pos, color);
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
        auto color = parseVector3fCanBeSingle(json["color"]);
        light = new ParallelLight(Ray(pos, dir), color);
    }
    else if(json["type"] == "spot")
    {
        auto pos = parseVector3f(json["pos"]);
        Vector3f dir;
        if(!json["dir"].isNull())
            dir = parseVector3f(json["dir"]);
        else if(!json["target"].isNull())
            dir = parseVector3f(json["target"]) - pos;
        else
            throw std::invalid_argument("SpotLight does not contain 'dir' or 'target'.");
        auto color = parseVector3fCanBeSingle(json["color"]);
        auto angle = json["angle"].asFloat();
        auto edgeAngle = json["edge_angle"].asFloat();
        light = new SpotLight(color, Ray(pos, dir), angle, edgeAngle);
    }
    else
        throw std::invalid_argument("LightSource type wrong.");
    light->name = json["name"].asString();
    light->enable = json.get("enable", true).asBool();
    return light;
}

ObjectMaterial * SceneParser::buildMaterial(Json::Value const &json) {
    if(json.isString()) {
        auto m = materialDict[json.asString()];
        if(m == nullptr)
            throw std::invalid_argument("Can not find material: " + json.asString());
        return m;
    }
    auto material = new ObjectMaterial();
    material->m.emission = parseVector3fCanBeSingle(json["emission"]);
//    material->m.ambient = parseVector3fCanBeSingle(json["diffuse"]);
    if(!json["ambient"].isNull())   // 若不设置，ambient = diffuse
        material->m.ambient = parseVector3fCanBeSingle(json["ambient"]);
    material->diffuse = buildTexture(json["diffuse"]);
    material->m.specular = parseVector3fCanBeSingle(json["specular"]);
    material->m.shininess = json["shininess"].asFloat();
    material->m.tdiffuse = parseVector3fCanBeSingle(json["tdiffuse"]);
    material->m.tspecular = parseVector3fCanBeSingle(json["tspecular"]);
    material->m.tshininess = json["tshininess"].asFloat();
    material->m.reflection = parseVector3fCanBeSingle(json["reflection"]);
    material->m.refraction = parseVector3fCanBeSingle(json["refraction"]);
    material->m.refractiveIndex = json.get("refractive_index", 1).asFloat();
    material->m.outRefractiveIndex = json.get("out_refractive_index", 1).asFloat();
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
    else if(json["type"] == "mesh")
    {
        auto file = json["obj_file"].asString();
        auto mesh = new TriangleMesh(file);
        mesh->faces.resize(20);
        std::cerr << *mesh << std::endl;
        shape = mesh;
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
    else if(json["type"] == "path_tracer")
    {
        auto rt = new PathTracer(world, camera);
        rt->setMaxDepth(json.get("depth", 2).asInt());
        rt->times = json.get("times", 5).asInt();
        renderer = rt;
    }
    renderer->super = json["super"].asBool();
    renderer->enableParallel = json.get("parallel", true).asBool();
    renderer->enableRecolor = json["recolor"].asBool();
    renderer->name = json["name"].asString();
    return renderer;
}

Texture *SceneParser::buildTexture(Json::Value const &json) {
    Texture* texture = nullptr;
    if(!json.isObject())
    {
        Color c = parseVector3fCanBeSingle(json);
        texture = new ConstTexture(c);
        return texture;
    }
    else if(json["type"] == "image")
    {
        auto file = json["file"].asString();
        texture = new ImageTexture(file);
    }
    else if(json["type"] == "grid")
    {
        int n = json["n"].asInt();
        Color a = Color::zero;
        Color b = Color(1, 1, 1);
        if(!json["a"].isNull())
            a = parseVector3fCanBeSingle(json["a"]);
        if(!json["b"].isNull())
            b = parseVector3fCanBeSingle(json["b"]);
        texture = new GridTexture(n, a, b);
    }
    else
        throw std::invalid_argument("Texture type wrong: " + json["name"].asString());
    texture->name = json["name"].asString();
    return texture;
}

UVMap *SceneParser::buildUVMap(Json::Value const &json) {
    if(json.isNull())
        return new UVMap();
    if(json["type"] == "sphere")
    {
        auto uvmap = new SphereMap();
        uvmap->pos = parseVector3fCanBeSingle(json["pos"]);
        return uvmap;
    }
    else
        throw std::invalid_argument("UVMap type wrong: " + json["name"].asString());
}
