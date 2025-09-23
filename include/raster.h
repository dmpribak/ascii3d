#pragma once

#include "linalg.h"

class TriangleRasterizer {
    public:
        TriangleRasterizer(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3);
        bool inside(Vec2<float> p);

        template <typename T>
        T interp(T a1, T a2, T a3, Vec2<float> p) {
            Vec2<float> uv = Vec2<float>(e2(p), e3(p)) / 
                             (e1(p) + e2(p) + e3(p));
            float u = uv.x;
            float v = uv.y;

            return (1-u-v)*a3 + u*a1 + v*a2;
        }

        template <typename T>
        T interp_pc(T a1, T a2, T a3, Vec2<float> p) {
            Vec2<float> uv = Vec2<float>(e2(p)/d2, e3(p)/d3) / 
                             (e1(p)/d1 + e2(p)/d2 + e3(p)/d3);
            float u = uv.x;
            float v = uv.y;

            return (1-u-v)*a3 + u*a1 + v*a2;
        }
    private:
        Vec2<float> n1, n2, n3;
        float c1, c2, c3;
        float d1, d2, d3;
    public:
        float e1(Vec2<float> p);
        float e2(Vec2<float> p);
        float e3(Vec2<float> p);
};
