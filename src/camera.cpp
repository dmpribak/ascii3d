#include "camera.h"
#include "linalg.h"

Camera::Camera(int H_, int W_, float f, const Vec3<float> C_, Mat3x3<float> R_): H(H_), W(W_) {
    float mK[3][3] = {
            {f,     0.,     (float)W_/2},
            {0.,    f,      (float)H_/2},
            {0.,    0.,     1.        }
            };
    K = Mat3x3<float>(mK);
    C = Vec3<float>(C_.x, C_.y, C_.z);

    M = Mat4x4<float>();
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            M[i][j] = R_[i][j];
        }
    }
    Vec3<float> t = R_ * -C_;
    M[0][3] = t.x;
    M[1][3] = t.y;
    M[2][3] = t.z;
}

Vec3<float> Camera::project(Vec3<float>& v) const {
    Vec4<float> v_hom = homogenize(v);
    Vec3<float> v_cam = dehomogenize(M*v_hom);
    Vec3<float> v_img_hom = K*v_cam;
    Vec2<float> v_img = dehomogenize(v_img_hom);
    Vec3<float> v_px = Vec3<float>((float)H-v_img.y, v_img.x, v_img_hom.z); // Includes depth
    
    return v_px;
}
