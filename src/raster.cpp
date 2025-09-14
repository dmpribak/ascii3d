#include "raster.h"
#include "linalg.h"

TriangleRasterizer::TriangleRasterizer(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3) {
    e1 = Vec2<float>(-(p2.y - p1.y), p2.x - p1.x);
    c1 = (p2.y - p1.y) * p1.x - (p2.x - p1.x) * p1.y;

    e2 = Vec2<float>(-(p3.y - p2.y), p3.x - p2.x);
    c2 = (p3.y - p2.y) * p2.x - (p3.x - p2.x) * p2.y;

    e3 = Vec2<float>(-(p1.y - p3.y), p1.x - p3.x);
    c3 = (p1.y - p3.y) * p3.x - (p1.x - p3.x) * p3.y;
}

bool TriangleRasterizer::inside(Vec2<float> p) {
    return dot(e1, p) + c1 >= 0 &&
           dot(e2, p) + c2 >= 0 &&
           dot(e3, p) + c3 >= 0;
}
