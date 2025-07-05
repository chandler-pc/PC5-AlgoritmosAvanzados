#pragma once
#include <cmath>

template<typename T>
struct Vector2 {
    T x, y;

    Vector2() : x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}

    template<typename U>
    Vector2(const Vector2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

    Vector2<T> operator+(const Vector2<T>& other) const {
        return { x + other.x, y + other.y };
    }

    Vector2<T> operator-(const Vector2<T>& other) const {
        return { x - other.x, y - other.y };
    }

    // 🔸 Multiplicación mantiene el tipo T
    Vector2<T> operator*(const T s) const {
        return { x * s, y * s };
    }

    // 🔹 División devuelve Vector2<float> SIEMPRE
    Vector2<float> operator/(const T s) const {
        return { static_cast<float>(x) / s, static_cast<float>(y) / s };
    }

    [[nodiscard]] float dot(const Vector2<T>& v) const {
        return static_cast<float>(x) * v.x + static_cast<float>(y) * v.y;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(static_cast<float>(x * x + y * y));
    }

    [[nodiscard]] Vector2<float> normalized() const {
        float len = length();
        return (len > 0) ? (*this) / len : Vector2<float>();
    }
};


template<typename T>
struct Vector3 {
    T x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    // Conversión entre tipos (ej. int -> float)
    template<typename U>
    Vector3(const Vector3<U>& other)
        : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), z(static_cast<T>(other.z)) {
    }

    Vector3<T> operator+(const Vector3<T>& v) const {
        return { x + v.x, y + v.y, z + v.z };
    }

    Vector3<T> operator-(const Vector3<T>& v) const {
        return { x - v.x, y - v.y, z - v.z };
    }

    // Multiplicación escalar conserva tipo T
    Vector3<T> operator*(const T s) const {
        return { x * s, y * s, z * s };
    }

    // División: siempre retorna Vector3<float> para evitar truncamiento
    Vector3<float> operator/(const T s) const {
        return {
            static_cast<float>(x) / s,
            static_cast<float>(y) / s,
            static_cast<float>(z) / s
        };
    }

	Vector3<T> operator+=(const Vector3<T>& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

    [[nodiscard]] float dot(const Vector3<T>& v) const {
        return static_cast<float>(x * v.x + y * v.y + z * v.z);
    }

    [[nodiscard]] Vector3<float> cross(const Vector3<T>& v) const {
        return {
            static_cast<float>(y * v.z - z * v.y),
            static_cast<float>(z * v.x - x * v.z),
            static_cast<float>(x * v.y - y * v.x)
        };
    }

    [[nodiscard]] float length() const {
        return std::sqrt(static_cast<float>(x * x + y * y + z * z));
    }

    [[nodiscard]] Vector3<float> normalized() const {
        float len = length();
        return (len > 0) ? (*this) / len : Vector3<float>();
    }
};

template<typename T>
struct Vector4 {
    T x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    template<typename U>
    Vector4(const Vector4<U>& other)
        : x(static_cast<T>(other.x)),
        y(static_cast<T>(other.y)),
        z(static_cast<T>(other.z)),
        w(static_cast<T>(other.w)) {
    }

    Vector4<T> operator+(const Vector4<T>& v) const {
        return { x + v.x, y + v.y, z + v.z, w + v.w };
    }

    Vector4<T> operator-(const Vector4<T>& v) const {
        return { x - v.x, y - v.y, z - v.z, w - v.w };
    }

    Vector4<T> operator*(const T s) const {
        return { x * s, y * s, z * s, w * s };
    }

    Vector4<float> operator/(const T s) const {
        return {
            static_cast<float>(x) / s,
            static_cast<float>(y) / s,
            static_cast<float>(z) / s,
            static_cast<float>(w) / s
        };
    }

    [[nodiscard]] float dot(const Vector4<T>& v) const {
        return static_cast<float>(x * v.x + y * v.y + z * v.z + w * v.w);
    }

    [[nodiscard]] float length() const {
        return std::sqrt(dot(*this));
    }

    [[nodiscard]] Vector4<float> normalized() const {
        float len = length();
        return (len > 0) ? (*this) / len : Vector4<float>();
    }
};
