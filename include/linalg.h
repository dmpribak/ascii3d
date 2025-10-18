#pragma once

#include <cmath>
#include <iostream>
#include <math.h>

template <typename T>
class Vec2 {
    public:
        T x, y;

        Vec2(): x(0), y(0) {}
        Vec2(T x_, T y_): x(x_), y(y_) {}

        Vec2<T> operator-() const {
            return Vec2<T>(-x, -y);
        }
};

template <typename T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2) {
    return Vec2<T>(
            v1.x - v2.x,
            v1.y - v2.y
            );
}

template <typename T>
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2) {
    return Vec2<T>(
            v1.x + v2.x,
            v1.y + v2.y
            );
}

template <typename T>
T dot(const Vec2<T>& v1, const Vec2<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

float norm(const Vec2<float>& v);

Vec2<float> operator/(const Vec2<float>& v, const float a);

template <typename T>
Vec2<T> operator*(const Vec2<T>& v, const T a) {
    return Vec2<T>(v.x*a, v.y*a);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
    return os << "[" << v.x << ", " << v.y << "]";
}

template <typename T>
class Vec3 {
    public:
        T x, y, z;

        Vec3(): x(0), y(0), z(0) {}
        Vec3(T x_, T y_, T z_): x(x_), y(y_), z(z_) {}

        Vec3<T> operator-() const {
            return Vec3<T>(-x, -y, -z);
        }
};

template <typename T>
Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2) {
    return Vec3<T>(
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
            );
}

template <typename T>
Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2) {
    return Vec3<T>(
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z
            );
}

template <typename T>
T dot(const Vec3<T>& v1, const Vec3<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template <typename T>
Vec3<T> cross(const Vec3<T>& v1, const Vec3<T>& v2) {
    return Vec3<T>(
            v1.y*v2.z - v1.z*v2.y, 
            v1.z*v2.x - v1.x*v2.z, 
            v1.x*v2.y - v1.y*v2.x
            );
}

float norm(const Vec3<float>& v);

Vec3<float> operator/(const Vec3<float>& v, const float a);

template <typename T>
Vec3<T> operator*(const Vec3<T>& v, const T a) {
    return Vec3<T>(v.x*a, v.y*a, v.z*a);
}

template <typename T>
Vec3<T> operator*(const T a, const Vec3<T>& v) {
    return Vec3<T>(v.x*a, v.y*a, v.z*a);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) {
    return os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

template <typename T>
class Vec4 {
    public:
        T x, y, z, w;

        Vec4(): x(0), y(0), z(0), w(0) {}
        Vec4(T x_, T y_, T z_, T w_): x(x_), y(y_), z(z_), w(w_) {}
};

template <typename T>
Vec4<T> operator-(const Vec4<T>& v1, Vec4<T>& v2) {
    return Vec4<T>(
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z,
            v1.w - v2.w
            );
}

template <typename T>
Vec4<T> operator+(const Vec4<T>& v1, const Vec4<T>& v2) {
    return Vec4<T>(
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z,
            v1.w + v2.w
            );
}

template <typename T>
T dot(const Vec4<T>& v1, const Vec4<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

float norm(Vec4<float> v);

Vec4<float> operator/(const Vec4<float>& v, const float a);

template <typename T>
Vec4<T> operator*(const Vec4<T>& v, const T a) {
    return Vec4<T>(v.x*a, v.y*a, v.z*a, v.w*a);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec4<T>& v) {
    return os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
}

template <typename U, size_t N>
class SqMat {
    public:       
        // Creates identity matrix by default
        SqMat() {
            for(size_t i = 0; i < N; ++i) {
                for(size_t j = 0; j < N; ++j) {
                    m[i][j] = (i == j) ? 1 : 0;
                }
            }
        }

        static SqMat Zero() {
            SqMat M;
            for(size_t i = 0; i < N; ++i) {
                for(size_t j = 0; j < N; ++j) {
                    M.m[i][j] = 0;
                }
            }
            
            return M;
        }

        SqMat(const U mat[N][N]) {
            for(size_t i = 0; i < N; ++i) {
                for(size_t j = 0; j < N; ++j) {
                    m[i][j] = mat[i][j];
                }
            }
        }

        SqMat<U, N> T() const {
            U mat[N][N];
            for(size_t i = 0; i < N; ++i) {
                for(size_t j = 0; j < N; ++j) {
                    mat[i][j] = m[j][i];
                }
            }
            return SqMat(mat);
        }

        U* operator[](size_t idx) {
            return m[idx];
        }

    private:
        U m[N][N];
};

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& os, SqMat<T, N> mat) {
    os << "[\n";
    for(size_t i = 0; i < N; ++i) {
        os << "[";
        for(size_t j = 0; j < N-1; ++j) {
            os << mat[i][j] << ", ";
        }
        os << mat[i][N-1] << "]\n";
    }
    os << "]";
    return os;
}

template <typename T>
using Mat3x3 = SqMat<T, 3>;

template <typename T>
using Mat4x4 = SqMat<T, 4>;

template <typename T>
Vec3<T> operator*(Mat3x3<T> M, Vec3<T> v) {
    return Vec3<T>(
            M[0][0]*v.x + M[0][1]*v.y + M[0][2]*v.z,
            M[1][0]*v.x + M[1][1]*v.y + M[1][2]*v.z,
            M[2][0]*v.x + M[2][1]*v.y + M[2][2]*v.z
            );
}

template <typename T, size_t N>
SqMat<T, N> operator*(SqMat<T, N> A, SqMat<T, N> B) {
    SqMat<T, N> C = SqMat<T, N>::Zero();
    for(size_t i = 0; i < N; ++i) {
        for(size_t j = 0; j < N; ++j) {
            for(size_t k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[j][k];
            }
        }
    }

    return C;
}

template <typename T>
Vec4<T> operator*(Mat4x4<T> M, Vec4<T> v) {
    return Vec4<T>(
            M[0][0]*v.x + M[0][1]*v.y + M[0][2]*v.z + M[0][3]*v.w,
            M[1][0]*v.x + M[1][1]*v.y + M[1][2]*v.z + M[1][3]*v.w,
            M[2][0]*v.x + M[2][1]*v.y + M[2][2]*v.z + M[2][3]*v.w,
            M[3][0]*v.x + M[3][1]*v.y + M[3][2]*v.z + M[3][3]*v.w
            );
}

template <typename T>
Vec4<T> homogenize(const Vec3<T> &v) {
    return Vec4<T>(v.x, v.y, v.z, 1.);
}

Vec3<float> dehomogenize(const Vec4<float>& v);
Vec2<float> dehomogenize(const Vec3<float>& v);

Mat3x3<float> rot_x(float theta);
Mat3x3<float> rot_y(float theta);
