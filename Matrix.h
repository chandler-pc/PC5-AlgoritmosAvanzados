#pragma once
#include "Vector.h"

struct Matrix4 {
    float m[4][4];

    Matrix4() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int k = 0; k < 4; ++k) {
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    result.m[col][row] += m[k][row] * other.m[col][k];
                }
            }
        }
        return result;
    }

    Vector4 operator*(const Vector4& v) const {
        Vector4 result;
        result.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
        result.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
        result.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
        result.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
        return result;
    }

    static Matrix4 identity() {
        return {};
    }
     
    static Matrix4 translate(const float x, const float y, const float z) {
        Matrix4 result = identity();
        result.m[3][0] = x;
        result.m[3][1] = y;
        result.m[3][2] = z;
        return result;
    }

    static Matrix4 scale(const float sx, const float sy, const float sz) {
        Matrix4 result = {};
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        result.m[2][2] = sz;
        result.m[3][3] = 1.0f;
        return result;
    }

    static Matrix4 rotate_x(const float angle_rad) {
        Matrix4 result = identity();
        const float c = std::cos(angle_rad);
        const float s = std::sin(angle_rad);
        result.m[1][1] = c;
        result.m[2][1] = -s;
        result.m[1][2] = s;
        result.m[2][2] = c;
        return result;
    }

    static Matrix4 rotate_y(const float angle_rad) {
        Matrix4 result = identity();
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        result.m[0][0] = c;
        result.m[2][0] = s;
        result.m[0][2] = -s;
        result.m[2][2] = c;
        return result;
    }

    static Matrix4 rotate_z(const float angle_rad) {
        Matrix4 result = identity();
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        result.m[0][0] = c;
        result.m[1][0] = -s;
        result.m[0][1] = s;
        result.m[1][1] = c;
        return result;
    }

    static Matrix4 perspective(const float fov_y_ad, const float aspect, const float z_near, const float z_far) {
        Matrix4 result = {};
        const float f = 1.0f / std::tan(fov_y_ad / 2.0f);
        result.m[0][0] = f / aspect;
        result.m[1][1] = f;
        result.m[2][2] = z_far / (z_far - z_near);
        result.m[3][2] = (-z_far * z_near) / (z_far - z_near);
        result.m[2][3] = 1.0f;
        result.m[3][3] = 0.0f;
        return result;
    }
};
