#pragma once

#include <cstddef>
#include <Eigen/Dense>

using Eigen::Vector3f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;

class Camera {
    public:
        size_t H, W;
        Matrix3f K;
        Matrix4f M;
        Vector3f C;
        Matrix3f R;
        
        Camera(size_t H_, size_t W_, float f, const Vector3f C_, const Matrix3f R_);

        static Camera Euler(size_t H_, size_t W_, float f, const Vector3f C_, const Vector3f angles);

        Vector3f project(Vector3f& v) const;

        void translate(const Vector3f& v);

        void set_R(const Matrix3f& Q);
};
