//
// Created by 王润基 on 2017/4/15.
//

#include "SceneParser.h"
#include "../Light/LightSource/PointLight.h"
#include "../Light/LightSource/ParallelLight.h"
#include "../Shapes/1D/BezierCurve.h"
#include "../Shapes/2D/Plane.h"
#include "../Shapes/2D/Triangle.h"
#include "../Shapes/2D/BezierSurface.h"
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
#include "../UVMaps/SphereMap.h"
#include "../Shapes/2D/Lathe.h"
#include "../Renderer/BiPathTracer.h"
#include "../Renderer/ProgressivePhotonMaps.h"
#include <fstream>

using namespace Json;

shared_ptr<World> SceneParser::load(const char *filePath) {
    std::ifstream fin(filePath);
    if(!fin)
        throw std::invalid_argument("Can not open scene file: " + string(filePath));
    fin >> json;
    fin.close();
    world = buildWorld(json["world"]);
    for(auto const& j: json["renderers"])
        rendererList.push_back(buildRenderer(j));
    return world;
}

unique_ptr<World> SceneParser::buildWorld(Json::Value const &json) {
    auto world = unique_ptr<World>(new World());
    for(auto const& j: json["materials"]) {
        auto material = buildMaterial(j);
        materialDict[material->name] = std::move(material);
    }
    for(auto const& j: json["objects"]) {
        if(j.get("enable", true).asBool() == false)
            continue;
        world->addObject(buildObject(j));
    }
    for(auto const& j: json["cameras"]) {
        world->addCamera(buildCamera(j));
    }
    for(auto const& j: json["lights"]) {
        world->addLight(buildLight(j));
    }
    world->setEnvColor(parseVector3fCanBeSingle(json["env_color"]));
    world->name = json["name"].asString();
    world->makeAABBTree();
    return world;
}

unique_ptr<Camera> SceneParser::buildCamera(Json::Value const &json) {
    Vector3f pos = parseVector3f(json["pos"]);
    Vector3f target = parseVector3f(json["target"]);
    Vector3f up = parseVector3f(json["up"]);
    int width = json["resolution"][0].asInt();
    int height = json["resolution"][1].asInt();
    float realw = json["realw"].asFloat();
    bool orthographic = json["orthographic"].asBool();
    auto camera = unique_ptr<Camera>(new Camera(pos, target, up, width, height, realw, orthographic));
    camera->name = json["name"].asString();
    camera->setAperture(json["aperture"].asFloat());
    camera->setFocalLength(json["focal"].asFloat());
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

unique_ptr<Object> SceneParser::buildObject(Json::Value const &json) {
    auto shape = buildShape(json["shape"]);
    auto material = buildMaterial(json["material"]);
    auto uvMap = buildUVMap(json["uvmap"]);
    auto name = json["name"].asString();
    auto ptr = unique_ptr<Object>(new Object(move(shape), move(material), move(uvMap), name));
    ptr->enable = json.get("enable", true).asBool();
    auto trans = parseTransform(json["transform"]);
    ptr->applyTransform(trans);
    return ptr;
}

unique_ptr<LightSource> SceneParser::buildLight(Json::Value const &json) {
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
        auto r = json.get("r", 1).asFloat();
        Vector3f dir;
        if(!json["dir"].isNull())
            dir = parseVector3f(json["dir"]);
        else if(!json["target"].isNull())
            dir = parseVector3f(json["target"]) - pos;
        else
            throw std::invalid_argument("ParallelLight does not contain 'dir' or 'target'.");
        auto color = parseVector3fCanBeSingle(json["color"]);
        light = new ParallelLight(Ray(pos, dir), r, color);
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
    light->shade = json.get("shade", true).asBool();
    light->enable = json.get("enable", true).asBool();
    return unique_ptr<LightSource>(light);
}

shared_ptr<ObjectMaterial> SceneParser::buildMaterial(Json::Value const &json) {
    if(json.isString()) {
        auto m = materialDict[json.asString()];
        if(m == nullptr)
            throw std::invalid_argument("Can not find material: " + json.asString());
        return m;
    }
    if(!json.isObject())
        throw std::invalid_argument("Material is not a string or object.");
    auto material = make_shared<ObjectMaterial>();
    material->m.emission = parseVector3fCanBeSingle(json["emission"]);
    material->diffuse = buildTexture(json["diffuse"]);
    material->ambient = json["ambient"].isNull()? material->diffuse: buildTexture(json["ambient"]); // 若不设置，ambient = diffuse
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

unique_ptr<Shape> SceneParser::buildShape(Json::Value const &json) {
    Shape* shape = nullptr;
    if(json["type"] == "lathe" || json["type"] == "bezier_surface")
    {
        ParameterSurface* ps;
        if(json["type"] == "lathe")
        {
            auto ray0 = parseVector3f(json["axis"][0]);
            auto ray1 = parseVector3f(json["axis"][1]);
            Ray axis(ray0, ray1);
            auto curve = std::dynamic_pointer_cast<ParameterCurve>(
                    shared_ptr<Shape>(buildShape(json["curve"])));
            if(curve == nullptr)
                throw std::invalid_argument("Lathe's curve is not a ParameterCurve.");
            ps = new Lathe(curve, axis);
        }
        else// if (json["type"] == "bezier_surface")
        {
            std::vector<Point> points;
            int m = json["points"].size();
            int n = json["points"][0].size();

            for(auto const& array: json["points"])
                for(auto const& j: array)
                    points.push_back(parseVector3f(j));
            ps = new BezierSurface(m, n, points);
        }
        shape = ps;
        int mm = json["mesh_size"][0].asInt();
        int mn = json["mesh_size"][1].asInt();
        ps->flip = json["flip"].asBool();
        ps->makeMesh(mm, mn);
        ps->setRendering(json.get("rendering", "iteration").asString());
        ps->iterTimes = json.get("iter_times", 5).asInt();
    }
    else if(json["type"] == "bezier_curve")
    {
        std::vector<Point> points;
        for(auto const& j: json["points"])
            points.push_back(parseVector3f(j));
        shape = new BezierCurve(points);
    }
    else if(json["type"] == "plane")
    {
        auto point = parseVector3f(json["point"]);
        auto normal = parseVector3f(json["normal"]);
        auto plane = new Plane(Ray(point, normal));
        plane->size = json.get("size", inf).asFloat();
        shape = plane;
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
        auto mesh = new TriangleMesh();
        if(!json["cut"].isNull())
            mesh->cutSize = json["cut"].asInt();
        mesh->normalInterpolation = json["normal_interpolation"].asBool();
        mesh->loadFromObj(file);
        std::cerr << *mesh << std::endl;
        shape = mesh;
    }
    else
        throw std::invalid_argument("Shape type wrong: " + json["name"].asString());
    shape->name = json["name"].asString();
    return unique_ptr<Shape>(shape);
}

unique_ptr<Renderer> SceneParser::buildRenderer(Json::Value const &json) {
    Renderer* renderer;
    auto camera = world->findCamera(json["camera"].asString());
    auto type = json["type"].asString();
    if(type == "light_projection")
    {
        renderer = new LightProjection(world, camera);
    }
    else if(type == "ray_tracer")
    {
        auto rt = new RayTracer(world, camera);
        rt->maxDepth = json.get("depth", 2).asInt();
        renderer = rt;
    }
    else if(type == "path_tracer")
    {
        auto rt = new PathTracer(world, camera);
        rt->maxDepth = json.get("depth", 2).asInt();
        renderer = rt;
    }
    else if(type == "bi_path_tracer")
    {
        auto rt = new BiPathTracer(world, camera);
        rt->maxDepth = json.get("depth", 2).asInt();
        renderer = rt;
    }
    else if(type == "ppm")
    {
        auto rt = new ProgressivePhotonMaps(world, camera);
        rt->maxDepth = json.get("depth", 2).asInt();
        renderer = rt;
    }
    else
        throw std::invalid_argument("No such renderer type: " + type);
    renderer->enable = json["enable"].asBool();
    renderer->super = json["super"].asBool();
    renderer->enableParallel = json.get("parallel", true).asBool();
    renderer->enableRecolor = json["recolor"].asBool();
    renderer->maxSampleTimes = json.get("times", 1).asInt();
    renderer->minSampleTimes = json.get("min_times", renderer->maxSampleTimes).asInt();
    renderer->s2 = json["s2"].asFloat();
    renderer->round = json.get("round", 1).asInt();
    renderer->name = json["name"].asString();
    return unique_ptr<Renderer>(renderer);
}

unique_ptr<Texture> SceneParser::buildTexture(Json::Value const &json) {
    Texture* texture = nullptr;
    if(!json.isObject())
    {
        Color c = parseVector3fCanBeSingle(json);
        texture = new ConstTexture(c);
        return unique_ptr<Texture>(texture);
    }
    else if(json["type"] == "image")
    {
        auto file = json["file"].asString();
        auto t = new ImageTexture(file);
        t->k = json.get("k", 1).asFloat();
        t->scale = json["scale"].isNull()? Vector3f(1,1,0): parseVector3fCanBeSingle(json["scale"]);
        texture = t;
    }
    else if(json["type"] == "grid")
    {
        float n = json["n"].asFloat();
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
    return unique_ptr<Texture>(texture);
}

unique_ptr<UVMap> SceneParser::buildUVMap(Json::Value const &json) {
    if(json.isNull())
        return nullptr;
    unique_ptr<UVMap> uvmap;
    if(json["type"] == "sphere")
    {
        auto sphere = new SphereMap();
        sphere->pos = parseVector3fCanBeSingle(json["pos"]);
        uvmap.reset(sphere);
    }
    else
        throw std::invalid_argument("UVMap type wrong: " + json["name"].asString());
    return uvmap;
}

Transform SceneParser::parseTransform(Json::Value const& json) {
    if(json.isNull())
        return Transform();
    auto pos = parseVector3f(json["pos"]);
    auto angle = parseVector3f(json["angle"]) / 360 * 2 * M_PI;
    auto scale = json.get("scale", 1).asFloat();
    // 先缩放，再旋转，后移动
    return Transform::move(pos) * Transform::rotate(angle) * Transform::scale(Vector3f(scale));
}

shared_ptr<Renderer> SceneParser::getRenderer() const {
    auto it = std::find_if(rendererList.begin(), rendererList.end(), [](shared_ptr<Renderer> const& r){return r->enable;});
    if(it == rendererList.end())
        throw std::invalid_argument("No enabled renderer.");
    return *it;
}
