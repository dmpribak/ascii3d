#pragma once

#include "linalg.h"

class TriangleRasterizer {
    public:
        TriangleRasterizer(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
        bool inside(Vec2<float> p);

        template <typename T>
        T interp(T a1, T a2, T a3, Vec2<float> p) {
            Vec2<float> uv = Vec2<float>(e2(p), e3(p)) / 
                             (e1(p) + e2(p) + e3(p));
            float u = uv.x;
            float v = uv.y;

            return (1-u-v)*a1 + u*a2 + v*a3;
        }
    private:
        Vec2<float> n1, n2, n3;
        float c1, c2, c3;
    public:
        float e1(Vec2<float> p);
        float e2(Vec2<float> p);
        float e3(Vec2<float> p);
};
