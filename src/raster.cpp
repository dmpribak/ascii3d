#include "raster.h"
#include "linalg.h"

TriangleRasterizer::TriangleRasterizer(Vec3<float> p1, Vec3<float> p2, Vec3<float> p3) {
    n1 = Vec2<float>(p2.y-p1.y, -(p2.x-p1.x));
    c1 = -(p2.y - p1.y) * p1.x + (p2.x - p1.x) * p1.y;

    n2 = Vec2<float>(p3.y-p2.y, -(p3.x-p2.x));
    c2 = -(p3.y - p2.y) * p2.x + (p3.x - p2.x) * p2.y;

    n3 = Vec2<float>(p1.y-p3.y, -(p1.x-p3.x));
    c3 = -(p1.y - p3.y) * p3.x + (p1.x - p3.x) * p3.y;

    d1 = p1.z;
    d2 = p2.z;
    d3 = p3.z;

    delta1 = p2-p1;
    delta2 = p3-p2;
    delta3 = p1-p3;
}

bool TriangleRasterizer::inside(Vec2<float> p) {
    return e1(p) >= 0 &&
           e2(p) >= 0 &&
           e3(p) >= 0;
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

RasterIterator TriangleRasterizer::xiter(Vec2<float> p) {
    return RasterIterator(
            e1(p), e2(p), e3(p),
            delta1.y, delta2.y, delta3.y
            );
}

RasterIterator::value_type RasterIterator::operator*() const {
    return inside;
}

RasterIterator& RasterIterator::operator++() {
    e1 += d1;
    e2 += d2;
    e3 += d3;
    inside = eval();

    return *this;
}

RasterIterator RasterIterator::operator++(int) {
    RasterIterator val = *this;
    ++(*this);

    return val;
}

RasterIterator& RasterIterator::operator--() {
    e1 -= d1;
    e2 -= d2;
    e3 -= d3;
    inside = eval();

    return *this;
}

RasterIterator RasterIterator::operator--(int) {
    RasterIterator val = *this;
    --(*this);

    return val;
}

bool RasterIterator::eval() {
    return e1 >= 0 &&
           e2 >= 0 &&
           e3 >= 0;
}

RasterIterator::RasterIterator(float e1, float e2, float e3, float d1, float d2, float d3)
    : e1(e1), e2(e2), e3(e3), d1(d1), d2(d2), d3(d3)
{
    inside = eval();
}
