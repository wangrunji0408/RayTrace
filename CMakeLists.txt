cmake_minimum_required(VERSION 3.7)
project(2RayTrace)

if(WIN32)
    set(OpenCV_DIR "D:\\Program Files\\opencv\\build")
endif()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

set(SOURCE_FILES IO/jsoncpp.cpp IO/json/json.h IO/json/json-forwards.h main.cpp Geometry/Vector3.cpp Geometry/Vector3.h Shapes/Shape.cpp Shapes/Shape.h Shapes/2D/Plane.cpp Shapes/2D/Plane.h Shapes/3D/Box.cpp Shapes/3D/Box.h Geometry/Ray.cpp Geometry/Ray.h Geometry/IRayCastable.h Geometry/Geometry.h Geometry/Geometry.cpp Shapes/3D/Sphere.cpp Shapes/3D/Sphere.h Shapes/3D/AxisBox.cpp Shapes/3D/AxisBox.h Geometry/IRayCastable.cpp Shapes/2D/Shape2D.cpp Shapes/2D/Shape2D.h Shapes/3D/Shape3D.cpp Shapes/3D/Shape3D.h Light/Light.cpp Light/Light.h Object/IntersectResult.cpp Object/IntersectResult.h Light/LightSource/LightSource.cpp Light/LightSource/LightSource.h Light/LightSource/PointLight.cpp Light/LightSource/PointLight.h Light/LightSource/ParallelLight.cpp Light/LightSource/ParallelLight.h Camera/Camera.cpp Camera/Camera.h Material/Material.cpp Material/Material.h Shapes/2D/Triangle.cpp Shapes/2D/Triangle.h Shapes/1D/Shape1D.cpp Shapes/1D/Shape1D.h Shapes/1D/BezierCurve.cpp Shapes/1D/BezierCurve.h Object/Object.cpp Object/Object.h Renderer/Renderer.cpp Renderer/Renderer.h Renderer/LightProjection.cpp Renderer/LightProjection.h World/World.cpp World/World.h Material/Material.cpp Material/Material.h IO/SceneParser.cpp IO/SceneParser.h Renderer/RayTracer.cpp Renderer/RayTracer.h Shapes/3D/TriangleMesh.cpp Shapes/3D/TriangleMesh.h UVMaps/UVMap.cpp UVMaps/UVMap.h UVMaps/SphereMap.cpp UVMaps/SphereMap.h Light/LightSource/SpotLight.cpp Light/LightSource/SpotLight.h Renderer/PathTracer.cpp Renderer/PathTracer.h Texture/Texture.cpp Texture/Texture.h Texture/ImageTexture.cpp Texture/ImageTexture.h Texture/GridTexture.cpp Texture/GridTexture.h Texture/ConstTexture.cpp Texture/ConstTexture.h Material/ObjectMaterial.cpp Material/ObjectMaterial.h DataStructure/KDTree.cpp DataStructure/KDTree.h Geometry/Transform.cpp Geometry/Transform.h DataStructure/AABBTree.cpp DataStructure/AABBTree.h Shapes/2D/BezierSurface.cpp Shapes/2D/BezierSurface.h)
add_executable(2RayTrace ${SOURCE_FILES})

target_link_libraries( 2RayTrace ${OpenCV_LIBS} )

configure_file(Scenes/scene.json scene.json)