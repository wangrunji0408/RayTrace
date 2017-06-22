//
// Created by wangrunji0408 on 2017/6/22.
//

#ifndef INC_2RAYTRACE_RECTLIGHT_H
#define INC_2RAYTRACE_RECTLIGHT_H


#include "LightSource.h"

class RectLight: public LightSource {
    Ray normal;

public:
    Light illuminate(Vector3f const &point) const override;
    Ray sample() const override;
};


#endif //INC_2RAYTRACE_RECTLIGHT_H
