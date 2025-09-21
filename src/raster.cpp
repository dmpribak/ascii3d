#include "raster.h"
#include "linalg.h"

TriangleRasterizer::TriangleRasterizer(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3) {
    n1 = Vec2<float>(-(p2.y-p1.y), p2.x-p1.x);
    c1 = (p2.y - p1.y) * p1.x + -(p2.x - p1.x) * p1.y;

    n2 = Vec2<float>(-(p3.y-p2.y), p3.x-p2.x);
    c2 = (p3.y - p2.y) * p2.x + -(p3.x - p2.x) * p2.y;

    n3 = Vec2<float>(-(p1.y-p3.y), p1.x-p3.x);
    c3 = (p1.y - p3.y) * p3.x + -(p1.x - p3.x) * p3.y;
}

bool TriangleRasterizer::inside(Vec2<float> p) {
    return e1(p) >= 0 &&
           e2(p) >= 0 &&
           e3(p) >= 0;
}

float TriangleRasterizer::interp(Vec3<float> a, Vec2<float> p) {
    Vec2<float> uv = Vec2<float>(e2(p), e3(p)) / 
                     (e1(p) + e2(p) + e3(p));
    float u = uv.x;
    float v = uv.y;

    return (1-u-v)*a.x + u*a.y + v*a.z;
}

float TriangleRasterizer::e1(Vec2<float> p) {
    return dot(n1, p) + c1;
}

float TriangleRasterizer::e2(Vec2<float> p) {
    return dot(n2, p) + c2;
}

float TriangleRasterizer::e3(Vec2<float> p) {
    return dot(n3, p) + c3;
}
