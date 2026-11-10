#ifndef KIT2D_SPRITE_VEC2_HPP
#define KIT2D_SPRITE_VEC2_HPP

#include <cmath>

namespace kit2d {

    class Vec2 {
    public:
        Vec2() : x(0.0f), y(0.0f) {
        }
        Vec2(float x, float y) : x(x), y(y) {
        }

        Vec2 add(const Vec2& v) const;
        Vec2 sub(const Vec2& v) const;
        Vec2 scale(float s) const;
        float dot(const Vec2& v) const;
        float length() const;
        float lengthSq() const;
        Vec2 normalize() const;

        Vec2 operator+(const Vec2& v) const;
        Vec2 operator-(const Vec2& v) const;
        Vec2 operator*(float s) const;
        Vec2& operator+=(const Vec2& v);
        Vec2& operator-=(const Vec2& v);
        Vec2& operator*=(float s);
        bool operator==(const Vec2& v) const;
        bool operator!=(const Vec2& v) const;

        float x, y;
    };

}

#endif
