cmake_minimum_required(VERSION 3.7)
project(2RayTrace)

if(WIN32)
    set(OpenCV_DIR "D:\\Program Files\\opencv\\build")
endif()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/IO/jsoncpp.cpp src/IO/json/json.h src/IO/json/json-forwards.h src/main.cpp src/Geometry/Vector3.cpp src/Geometry/Vector3.h src/Shapes/Shape.cpp src/Shapes/Shape.h src/Shapes/2D/Plane.cpp src/Shapes/2D/Plane.h src/Shapes/3D/Box.cpp src/Shapes/3D/Box.h src/Geometry/Ray.cpp src/Geometry/Ray.h src/Geometry/IRayCastable.h src/Geometry/Geometry.h src/Geometry/Geometry.cpp src/Shapes/3D/Sphere.cpp src/Shapes/3D/Sphere.h src/Shapes/3D/AxisBox.cpp src/Shapes/3D/AxisBox.h src/Geometry/IRayCastable.cpp src/Shapes/2D/Shape2D.cpp src/Shapes/2D/Shape2D.h src/Shapes/3D/Shape3D.cpp src/Shapes/3D/Shape3D.h src/Light/Light.cpp src/Light/Light.h src/Geometry/IntersectInfo.cpp src/Geometry/IntersectInfo.h src/Light/LightSource/LightSource.cpp src/Light/LightSource/LightSource.h src/Light/LightSource/PointLight.cpp src/Light/LightSource/PointLight.h src/Light/LightSource/ParallelLight.cpp src/Light/LightSource/ParallelLight.h src/Camera/Camera.cpp src/Camera/Camera.h src/Material/Material.cpp src/Material/Material.h src/Shapes/2D/Triangle.cpp src/Shapes/2D/Triangle.h src/Shapes/1D/Shape1D.cpp src/Shapes/1D/Shape1D.h src/Shapes/1D/BezierCurve.cpp src/Shapes/1D/BezierCurve.h src/Object/Object.cpp src/Object/Object.h src/Renderer/Renderer.cpp src/Renderer/Renderer.h src/Renderer/LightProjection.cpp src/Renderer/LightProjection.h src/World/World.cpp src/World/World.h src/Material/Material.cpp src/Material/Material.h src/IO/SceneParser.cpp src/IO/SceneParser.h src/Renderer/RayTracer.cpp src/Renderer/RayTracer.h src/Shapes/3D/TriangleMesh.cpp src/Shapes/3D/TriangleMesh.h src/UVMaps/UVMap.cpp src/UVMaps/UVMap.h src/UVMaps/SphereMap.cpp src/UVMaps/SphereMap.h src/Light/LightSource/SpotLight.cpp src/Light/LightSource/SpotLight.h src/Renderer/PathTracer.cpp src/Renderer/PathTracer.h src/Texture/Texture.cpp src/Texture/Texture.h src/Texture/ImageTexture.cpp src/Texture/ImageTexture.h src/Texture/GridTexture.cpp src/Texture/GridTexture.h src/Texture/ConstTexture.cpp src/Texture/ConstTexture.h src/Material/ObjectMaterial.cpp src/Material/ObjectMaterial.h src/DataStructure/KDTree.cpp src/DataStructure/KDTree.h src/Geometry/Transform.cpp src/Geometry/Transform.h src/DataStructure/AABBTree.cpp src/DataStructure/AABBTree.h src/Shapes/2D/BezierSurface.cpp src/Shapes/2D/BezierSurface.h src/Shapes/2D/ParameterSurface.cpp src/Shapes/2D/ParameterSurface.h src/Shapes/2D/Lathe.cpp src/Shapes/2D/Lathe.h src/Renderer/BiPathTracer.cpp src/Renderer/BiPathTracer.h src/Light/LightSource/RectLight.cpp src/Light/LightSource/RectLight.h)
add_executable(2RayTrace ${SOURCE_FILES})

target_link_libraries( 2RayTrace ${OpenCV_LIBS} )

configure_file(data/scenes/scene.json scene.json)