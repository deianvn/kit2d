#include "../../include/kit2d/sprite/Vec2.hpp"

namespace kit2d {

    Vec2 Vec2::add(const Vec2& v) const {
        return Vec2 { x + v.x, y + v.y };
    }
    Vec2 Vec2::sub(const Vec2& v) const {
        return Vec2 { x - v.x, y - v.y };
    }
    Vec2 Vec2::scale(float s) const {
        return Vec2 { x * s, y * s };
    }
    float Vec2::dot(const Vec2& v) const {
        return x * v.x + y * v.y;
    }
    float Vec2::lengthSq() const {
        return x * x + y * y;
    }
    float Vec2::length() const {
        return std::sqrt(lengthSq());
    }

    Vec2 Vec2::normalize() const {
        float len = length();
        if (len == 0.0f)
            return Vec2 {};
        return Vec2 { x / len, y / len };
    }

    Vec2 Vec2::operator+(const Vec2& v) const {
        return add(v);
    }
    Vec2 Vec2::operator-(const Vec2& v) const {
        return sub(v);
    }
    Vec2 Vec2::operator*(float s) const {
        return scale(s);
    }

    Vec2& Vec2::operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2& Vec2::operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2& Vec2::operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }

    bool Vec2::operator==(const Vec2& v) const {
        return x == v.x && y == v.y;
    }
    bool Vec2::operator!=(const Vec2& v) const {
        return !(*this == v);
    }

}
