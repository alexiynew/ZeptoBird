//
// Created by alex on 21.09.15.
//

#ifndef ZEPTOBIRD_VECTOR2_H
#define ZEPTOBIRD_VECTOR2_H

#include <cmath>

namespace ZB {

    template<typename T>
    struct Vec2 {
        T x, y;

        Vec2() : x(0), y(0) { }

        Vec2(const T &xx, const T &yy) : x(xx), y(yy) { }

        template<typename U>
        Vec2(const Vec2<U> &v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }

        template<typename U>
        Vec2 &operator=(const Vec2<U> &v) {
            x = static_cast<T>(v.x);
            y = static_cast<T>(v.y);
            return *this;
        }

        Vec2 &operator+=(const Vec2<T> &v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        Vec2 &operator-=(const Vec2<T> &v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        Vec2 &operator*=(const Vec2<T> &v) {
            x *= v.x;
            y *= v.y;
            return *this;
        }

        Vec2 &operator/=(const Vec2<T> &v) {
            x /= v.x;
            y /= v.y;
            return *this;
        }

        Vec2 &operator+=(const T &s) {
            x += s;
            y += s;
            return *this;
        }

        Vec2 &operator-=(const T &s) {
            x -= s;
            y -= s;
            return *this;
        }

        Vec2 &operator*=(const T &s) {
            x *= s;
            y *= s;
            return *this;
        }

        Vec2 &operator/=(const T &s) {
            x /= s;
            y /= s;
            return *this;
        }
    };

    template<typename T>
    Vec2<T> operator+(const Vec2<T> &a, const Vec2<T> &b) {
        return Vec2<T>(a) += b;
    }

    template<typename T>
    Vec2<T> operator-(const Vec2<T> &a, const Vec2<T> &b) {
        return Vec2<T>(a) -= b;
    }

    template<typename T>
    Vec2<T> operator*(const Vec2<T> &a, const Vec2<T> &b) {
        return Vec2<T>(a) *= b;
    }

    template<typename T>
    Vec2<T> operator/(const Vec2<T> &a, const Vec2<T> &b) {
        return Vec2<T>(a) /= b;
    }

    template<typename T>
    Vec2<T> operator+(const Vec2<T> &a, const T &s) {
        return Vec2<T>(a) += s;
    }

    template<typename T>
    Vec2<T> operator-(const Vec2<T> &a, const T &s) {
        return Vec2<T>(a) -= s;
    }

    template<typename T>
    Vec2<T> operator*(const Vec2<T> &a, const T &s) {
        return Vec2<T>(a) *= s;
    }

    template<typename T>
    Vec2<T> operator/(const Vec2<T> &a, const T &s) {
        return Vec2<T>(a) /= s;
    }

    template<typename T>
    Vec2<T> operator+(const T &s, const Vec2<T> &a) {
        return Vec2<T>(s, s) += a;
    }

    template<typename T>
    Vec2<T> operator-(const T &s, const Vec2<T> &a) {
        return Vec2<T>(s, s) -= a;
    }

    template<typename T>
    Vec2<T> operator*(const T &s, const Vec2<T> &a) {
        return Vec2<T>(s, s) *= a;
    }

    template<typename T>
    Vec2<T> operator/(const T &s, const Vec2<T> &a) {
        return Vec2<T>(s, s) /= a;
    }

    template <typename T>
    double dot(const Vec2<T> &a, const Vec2<T> &b) {
        return a.x * b.x + a.y * b.y;
    }

    template <typename T>
    double length(const Vec2<T> &a) {
        return std::sqrt(dot(a, a));
    }

    template<typename T>
    Vec2<T> normalize(const Vec2<T> &a) {
        double l = length(a);
        return Vec2<T>(a.x / l, a.y / l);
    }

    using Vec2f = Vec2<float>;

} // namespace ZB


#endif //ZEPTOBIRD_VECTOR2_H
