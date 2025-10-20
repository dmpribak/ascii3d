#include "linalg.h"

#include <math.h>

float norm(const Vec2<float>& v) {
    return std::sqrt(v.x*v.x + v.y*v.y);
}

Vec2<float> operator/(const Vec2<float>& v, const float a) {
    return Vec2<float>(v.x/a, v.y/a);
}

float norm(const Vec3<float>& v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec3<float> operator/(const Vec3<float>& v, const float a) {
    return Vec3<float>(v.x/a, v.y/a, v.z/a);
}

float norm(const Vec4<float>& v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vec4<float> operator/(const Vec4<float>& v, const float a) {
    return Vec4<float>(v.x/a, v.y/a, v.z/a, v.w/a);
}

Vec3<float> dehomogenize(const Vec4<float>& v) {
    return Vec3<float>(v.x, v.y, v.z) / v.w;
}

Vec2<float> dehomogenize(const Vec3<float>& v) {
    return Vec2<float>(v.x, v.y) / v.z;
}

Mat3x3<float> rot_x(float theta) {
    theta *= 3.14159265/180.;
    float m[3][3] = {
        {1.,    0.,             0.         },
        {0.,    cos(theta),     -sin(theta)},
        {0.,    sin(theta),     cos(theta) }
    };

    return Mat3x3<float>(m);

}

Mat3x3<float> rot_y(float theta) {
    theta *= 3.14159265/180.;
    float m[3][3] = {
        {cos(theta),    0.,     sin(theta)},
        {0.,            1.,     0         },
        {-sin(theta),   0.,     cos(theta)}
    };

    return Mat3x3<float>(m);

}

Mat3x3<float> euler(Vec3<float> angles) {
    return rot_x(angles.x) * rot_y(angles.y); 
}
