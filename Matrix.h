#pragma once
#include "Vector.h"

struct Matrix4 {
	float m[4][4] = {};

    Matrix4() {
        for (auto& i : m)
        {
            for (float& j : i)
            {
	            j = 0.0f;
            }
        }
    }

    Matrix4(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33)
    {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }
    
    static Matrix4 identity() {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
		return result;
    }

    static Matrix4 zero() {
        return {};
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result = Matrix4::zero();
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                for (int k= 0; k< 4; ++k) {
                    result.m[row][col] += m[row][k] * other.m[k][col];
                }
            }
        }
        return result;
    }

    Vector4<float> operator*(const Vector4<float>& v) const {
        Vector4<float> result;
        result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
        result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
        result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
        result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
        return result;
    }




    static Matrix4 translate(const float x, const float y, const float z) {
        Matrix4 result = identity();
        result.m[0][3] = x;
        result.m[1][3] = y;
        result.m[2][3] = z;
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
        result.m[2][1] = s;
        result.m[1][2] = -s;
        result.m[2][2] = c;
        return result;
    }

    static Matrix4 rotate_y(const float angle_rad) {
        Matrix4 result = identity();
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        result.m[0][0] = c;
        result.m[2][0] = -s;
        result.m[0][2] = s;
        result.m[2][2] = c;
        return result;
    }

    static Matrix4 rotate_z(const float angle_rad) {
        Matrix4 result = identity();
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        result.m[0][0] = c;
        result.m[1][0] = s;
        result.m[0][1] = -s;
        result.m[1][1] = c;
        return result;
    }

    static Matrix4 perspective(float fov_y_rad, float aspect, float z_near, float z_far) {
        Matrix4 result = {};
        float f = 1.0f / std::tan(fov_y_rad / 2.0f);

        result.m[0][0] = f / aspect;
        result.m[1][1] = f;
        result.m[2][2] = (z_far + z_near) / (z_near - z_far);
        result.m[2][3] = (2.0f * z_far * z_near) / (z_near - z_far);
        result.m[3][2] = -1.0f;
        result.m[3][3] = 0.0f;

        return result;
    }


    static Matrix4 look_at(const Vector3<float>& eye, const Vector3<float>& target, const Vector3<float>& y) {
        Vector3<float> fwd = (eye - target).normalized();
        Vector3<float> side = (fwd *-1).cross(y).normalized();
        Vector3<float> up = side.cross(fwd * -1).normalized();;

        Matrix4 result = Matrix4::identity();

        result.m[0][0] = side.x;
        result.m[0][1] = side.y;
        result.m[0][2] = side.z;

        result.m[1][0] = up.x;
        result.m[1][1] = up.y;
        result.m[1][2] = up.z;

        result.m[2][0] = -fwd.x;
        result.m[2][1] = -fwd.y;
        result.m[2][2] = -fwd.z;

        result.m[0][3] = -side.dot(eye);
        result.m[1][3] = -up.dot(eye);
        result.m[2][3] = -1*(fwd*-1).dot(eye);

        return result;
    }

};
