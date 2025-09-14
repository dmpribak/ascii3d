#include "linalg.h"

class Camera {
    public:
        int H, W;
        Mat3x3<float> K;
        Mat4x4<float> M;
        Vec3<float> C;
        
        Camera(int H_, int W_, float f, const Vec3<float> C_, const Mat3x3<float> R_);

        Vec3<float> project(Vec3<float>& v) const;
};
