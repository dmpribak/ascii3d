#include "camera.h"
#include "linalg.h"
#include <cstddef>

#include <Eigen/Dense>

using Eigen::Vector3f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;

Camera::Camera(size_t H_, size_t W_, float f, const Vector3f C_, Matrix3f R_): H(H_), W(W_) {
    float s = (float)H_/(float)W_;
    float mK[3*3] = {
            f,     0.,     (float)W_/2,
            0.,    f*s,      (float)H_/2,
            0.,    0.,     1.        
            };
    K = Matrix3f(mK).transpose();
    C = Vector3f(C_.x(), C_.y(), C_.z());

    M = Matrix4f::Identity();
    for(size_t i = 0; i < 3; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            M(i,j) = R_(i,j);
            R(i,j) = R_(i,j);
        }
    }
    Vector3f t = R_ * -C_;
    M(0,3) = t.x();
    M(1,3) = t.y();
    M(2,3) = t.z();
}

Vector3f Camera::project(Vector3f& v) const {
    Vector4f v_hom = homogenize3f(v);
    Vector4f v_cam_hom = M*v_hom;
    Vector3f v_cam = dehomogenize4f(v_cam_hom);
    Vector3f v_img_hom = K*v_cam;
    Vector2f v_img = dehomogenize3f(v_img_hom);
    Vector3f v_px = Vector3f(v_img.x(), v_img.y(), v_img_hom.z()); // Includes depth
    
    return v_px;
}

void Camera::translate(const Vector3f& v) {
    C = C+v;
    Vector3f t = R * -C;
    M(0,3) = t.x();
    M(1,3) = t.y();
    M(2,3) = t.z();
}

void Camera::set_R(const Matrix3f& Q) {
    for(size_t i = 0; i < 3; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            M(i,j) = Q(i,j);
            R(i,j) = Q(i,j);
        }
    }
    Vector3f t = R * -C;
    M(0,3) = t.x();
    M(1,3) = t.y();
    M(2,3) = t.z();
}

Camera Camera::Euler(size_t H_, size_t W_, float f, const Vector3f C_, const Vector3f angles) {
    Matrix3f R = rot_x(angles.x()) * rot_y(angles.y());

    return Camera(H_, W_, f, C_, R.transpose());
}
