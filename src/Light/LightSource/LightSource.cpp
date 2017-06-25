//
// Created by 王润基 on 2017/4/8.
//

#include <vector>
#include "../../Geometry/Vector3.h"
#include "LightSource.h"

std::vector <Light, std::allocator<Light>> LightSource::illuminates(const Vector3f &point) const {
    return std::vector< Light > (1, illuminate(point));
}
