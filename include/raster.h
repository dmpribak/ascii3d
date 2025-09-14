#include "linalg.h"

class TriangleRasterizer {
    public:
        TriangleRasterizer(Vec2<float> p1, Vec2<float> p2, Vec2<float> p3);
        bool inside(Vec2<float> p);
    private:
        Vec2<float> e1, e2, e3;
        float c1, c2, c3;
};
