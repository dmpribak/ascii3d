#pragma once

#include "linalg.h"
#include <cstddef>

class Camera {
    public:
        size_t H, W;
        Mat3x3<float> K;
        Mat4x4<float> M;
        Vec3<float> C;
        Mat3x3<float> R;
        
        Camera(size_t H_, size_t W_, float f, const Vec3<float> C_, const Mat3x3<float> R_);

        static Camera Euler(size_t H_, size_t W_, float f, const Vec3<float> C_, const Vec3<float> angles);

        Vec3<float> project(Vec3<float>& v) const;

        void translate(const Vec3<float>& v);

        void set_R(const Mat3x3<float>& Q);
};
