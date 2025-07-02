#pragma once
#include <cmath>

struct Vector2 {

    float x, y;

    Vector2() : x(0), y(0) {}

    Vector2(const float x, const float y) : x(x), y(y) {}

	Vector2(const int x, const int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}

    Vector2 operator+(const Vector2& other) const
    {
	    return { x + other.x, y + other.y };
    }

    Vector2 operator-(const Vector2& other) const
    {
	    return { x - other.x, y - other.y };
    }

    Vector2 operator*(const float s) const
    {
	    return { x * s, y * s };
    }

    Vector2 operator/(const float s) const
    {
	    return { x / s, y / s };
    }

    [[nodiscard]] float dot(const Vector2& v) const
    {
	    return x * v.x + y * v.y;
    }

    [[nodiscard]] float length() const
    {
	    return std::sqrt(x * x + y * y);
    }

    [[nodiscard]] Vector2 normalized() const {
        const float len = length();
        return (len > 0) ? (*this) / len : Vector2();
    }
};

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const
    {
	    return { x + v.x, y + v.y, z + v.z };
    }

    Vector3 operator-(const Vector3& v) const
    {
	    return { x - v.x, y - v.y, z - v.z };
    }

    Vector3 operator*(const float s) const
    {
	    return { x * s, y * s, z * s };
    }

    Vector3 operator/(const float s) const
    {
	    return { x / s, y / s, z / s };
    }

    [[nodiscard]] float dot(const Vector3& v) const
    {
	    return x * v.x + y * v.y + z * v.z;
    }

    [[nodiscard]] Vector3 cross(const Vector3& v) const {
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }

    [[nodiscard]] float length() const
    {
	    return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] Vector3 normalized() const {
        const float len = length();
        return (len > 0) ? (*this) / len : Vector3();
    }
};

struct Vector4 {

    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}

    Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

    Vector4 operator+(const Vector4& v) const
    {
	    return { x + v.x, y + v.y, z + v.z, w + v.w };
    }

    Vector4 operator-(const Vector4& v) const
    {
	    return { x - v.x, y - v.y, z - v.z, w - v.w };
    }

    Vector4 operator*(const float s) const
    {
	    return { x * s, y * s, z * s, w * s };
    }

    Vector4 operator/(const float s) const
    {
	    return { x / s, y / s, z / s, w / s };
    }

    [[nodiscard]] float dot(const Vector4& v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

	[[nodiscard]] float length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    [[nodiscard]] Vector4 normalized() const {
        const float len = length();
        return (len > 0) ? (*this) / len : Vector4();
    }
};