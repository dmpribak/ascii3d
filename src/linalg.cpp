#include "linalg.h"

#include <math.h>
#include <Eigen/Dense>

using Eigen::Vector2f;
using Eigen::Vector3f;
using Eigen::Vector4f;
using Eigen::Matrix3f;

Vector4f homogenize3f(const Vector3f &v) {
    return Vector4f(v.x(), v.y(), v.z(), 1.);
}

Vector3f dehomogenize4f(const Vector4f& v) {
    return Vector3f(v.x(), v.y(), v.z()) / v.w();
}

Vector2f dehomogenize3f(const Vector3f& v) {
    return Vector2f(v.x(), v.y()) / v.z();
}

Matrix3f rot_x(float theta) {
    theta *= 3.14159265/180.;
    float m[3*3] = {
        1.,    0.,             0.,
        0.,    cos(theta),     -sin(theta),
        0.,    sin(theta),     cos(theta) 
    };

    return Matrix3f(m).transpose();

}

Matrix3f rot_y(float theta) {
    theta *= 3.14159265/180.;
    float m[3*3] = {
        cos(theta),    0.,     sin(theta),
        0.,            1.,     0         ,
        -sin(theta),   0.,     cos(theta)
    };

    return Matrix3f(m).transpose();

}

Matrix3f euler(Vector3f angles) {
    return rot_x(angles(0)) * rot_y(angles(1)); 
}
