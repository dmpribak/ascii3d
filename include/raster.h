#include "linalg.h"

class TriangleRasterizer {
    public:
        TriangleRasterizer(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
        bool inside(Vec2<float> p);
        float interp(Vec3<float> a, Vec2<float> p);
    private:
        Vec2<float> n1, n2, n3;
        float c1, c2, c3;
    public:
        float e1(Vec2<float> p);
        float e2(Vec2<float> p);
        float e3(Vec2<float> p);
};
