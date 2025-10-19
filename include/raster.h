#pragma once

#include "linalg.h"

class RasterIterator {
    public:
        using value_type = bool;

        value_type operator*() const;

        RasterIterator& operator++();
        RasterIterator operator++(int);

        RasterIterator& operator--();
        RasterIterator operator--(int);

        friend class TriangleRasterizer;

    private:
        float e1, e2, e3;
        float d1, d2, d3;
        bool inside;

        bool eval();

        RasterIterator(float e1, float e2, float e3, 
                       float d1, float d2, float d3);
};

class TriangleRasterizer {
    public:
        TriangleRasterizer(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3);
        bool inside(Vec2<float> p);

        template <typename T>
        T interp(T a1, T a2, T a3, Vec2<float> p);

        template <typename T>
        T interp_pc(T a1, T a2, T a3, Vec2<float> p);

        float e1(Vec2<float> p);
        float e2(Vec2<float> p);
        float e3(Vec2<float> p);

        RasterIterator xiter(Vec2<float> p);

    private:
        Vec2<float> n1, n2, n3;
        float c1, c2, c3;
        float d1, d2, d3;
        Vec3<float> delta1, delta2, delta3;
};

template <typename T>
T TriangleRasterizer::interp(T a1, T a2, T a3, Vec2<float> p) {
    Vec2<float> uv = Vec2<float>(e2(p), e3(p)) / 
                     (e1(p) + e2(p) + e3(p));
    float u = uv.x;
    float v = uv.y;

    return (1-u-v)*a3 + u*a1 + v*a2;
}

template <typename T>
T TriangleRasterizer::interp_pc(T a1, T a2, T a3, Vec2<float> p) {
    Vec2<float> uv = Vec2<float>(e2(p)/d2, e3(p)/d3) / 
                     (e1(p)/d1 + e2(p)/d2 + e3(p)/d3);
    float u = uv.x;
    float v = uv.y;

    return (1-u-v)*a3 + u*a1 + v*a2;
}
