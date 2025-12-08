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

        template <typename T>
        T interp(T a1, T a2, T a3);

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
        TriangleRasterizer(Vector3f p1, Vector3f p2, Vector3f p3);
        bool inside(Vector2f p);

        template <typename T>
        T interp(T a1, T a2, T a3, Vector2f p);

        template <typename T>
        T interp_pc(T a1, T a2, T a3, Vector2f p);

        float e1(const Vector2f p);
        float e2(const Vector2f p);
        float e3(const Vector2f p);

        RasterIterator xiter(const Vector2f p);

    private:
        Vector2f n1, n2, n3;
        float c1, c2, c3;
        float d1, d2, d3;
        Vector3f delta1, delta2, delta3;
};

template <typename T>
T RasterIterator::interp(T a1, T a2, T a3) {
    Vector2f uv = Vector2f(e2, e3) / 
                     (e1 + e2 + e3);
    float u = uv.x();
    float v = uv.y();

    return (1-u-v)*a3 + u*a1 + v*a2;
}

template <typename T>
T TriangleRasterizer::interp(T a1, T a2, T a3, Vector2f p) {
    // Vector2f uv = Vector2f(e2(p), e3(p)) / 
    //                  (e1(p) + e2(p) + e3(p));
    // float u = uv.x();
    // float v = uv.y();
    //
    float denom = 1./(e1(p) + e2(p) + e3(p));
    float u = e2(p) * denom;
    float v = e3(p) * denom;

    return (1-u-v)*a3 + u*a1 + v*a2;
}

template <typename T>
T TriangleRasterizer::interp_pc(T a1, T a2, T a3, Vector2f p) {
    Vector2f uv = Vector2f(e2(p)/d2, e3(p)/d3) / 
                     (e1(p)/d1 + e2(p)/d2 + e3(p)/d3);
    float u = uv.x();
    float v = uv.y();

    return (1-u-v)*a3 + u*a1 + v*a2;
}
