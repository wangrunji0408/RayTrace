//
// Created by 王润基 on 2017/4/19.
//

#include "ObjectMaterial.h"

Material ObjectMaterial::getMaterial(Vector3f const &uv) const {
    Material mm = m;
    if(diffuse != nullptr)
        mm.diffuse = (*diffuse)(uv);
    return mm;
}
