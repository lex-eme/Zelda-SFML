#pragma once

#include <iostream>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float xin, float yin);
    Vec2(int xin, int yin);

    bool operator==(const Vec2& rhs) const;
    bool operator!=(const Vec2& rhs) const;

    Vec2 operator+(const Vec2& rhs) const;
    Vec2 operator-(const Vec2& rhs) const;
    Vec2 operator*(float val) const;
    Vec2 operator/(float val) const;

    Vec2& operator+=(const Vec2& rhs);
    Vec2& operator-=(const Vec2& rhs);
    Vec2& operator*=(float val);
    Vec2& operator/=(float val);

    float dist(const Vec2& rhs) const;
    float distSq(const Vec2& rhs) const;
    float mag() const;
    float cross(const Vec2& rhs) const;
    Vec2& normalize();
    Vec2& setMag(float len);
    Vec2& rotate(float theta);
    Vec2& lerp(const Vec2& other, float amount);

    friend std::ostream& operator<<(std::ostream& os, const Vec2& dt);
};
