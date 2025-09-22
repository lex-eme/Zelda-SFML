#include "Vec2.h"

#include <cmath>

constexpr auto PI = 3.14159265f;

Vec2::Vec2(const float xin, const float yin)
    : x(xin), y(yin) {
}

Vec2::Vec2(const int xin, const int yin)
    : x(static_cast<float>(xin)), y(static_cast<float>(yin)) {
}

bool Vec2::operator==(const Vec2& rhs) const {
    return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(const Vec2& rhs) const {
    return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator+(const Vec2& rhs) const {
    return {x + rhs.x, y + rhs.y};
}

Vec2 Vec2::operator-(const Vec2& rhs) const {
    return {x - rhs.x, y - rhs.y};
}

Vec2 Vec2::operator*(const float val) const {
    return {x * val, y * val};
}

Vec2 Vec2::operator/(const float val) const {
    return {x / val, y / val};
}

Vec2& Vec2::operator+=(const Vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vec2& Vec2::operator*=(const float val) {
    x *= val;
    y *= val;
    return *this;
}

Vec2& Vec2::operator/=(const float val) {
    x /= val;
    y /= val;
    return *this;
}

float Vec2::dist(const Vec2& rhs) const {
    return sqrtf(distSq(rhs));
}

float Vec2::distSq(const Vec2& rhs) const {
    const float dx = rhs.x - x;
    const float dy = rhs.y - y;
    return dx * dx + dy * dy;
}

float Vec2::mag() const {
    return sqrt(x * x + y * y);
}

float Vec2::cross(const Vec2& rhs) const {
    return x * rhs.y - y * rhs.x;
}

Vec2& Vec2::normalize() {
    float m = mag();

    if (m != 0.0f && m != 1.0f) {
        *this /= m;
    }

    return *this;
}

Vec2& Vec2::setMag(float len) {
    normalize();
    *this *= len;
    return *this;
}

Vec2& Vec2::rotate(float theta) {
    const float t = x;
    const float angle = theta * PI / 180.0f;
    x = x * cos(angle) - y * sin(angle);
    y = t * sin(angle) + y * cos(angle);
    return *this;
}

Vec2& Vec2::lerp(const Vec2& other, const float amount) {
    *this = *this + (other - *this) * amount;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Vec2& dt) {
    os << '(' << dt.x << ',' << dt.y << ')';
    return os;
}
