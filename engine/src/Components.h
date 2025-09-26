#pragma once

#include <SFML/Graphics.hpp>

#include "Vec2.h"
#include "Animation.h"

struct Component {
    bool has = false;
};

struct CTransform : Component {
    Vec2 pos = {0.0f, 0.0f};
    Vec2 prevPos = {0.0f, 0.0f};
    Vec2 scale = {1.0f, 1.0f};
    Vec2 velocity = {0.0f, 0.0f};
    Vec2 facing = {0.0f, 1.0f};
    sf::Angle angle;

    CTransform() = default;

    explicit CTransform(const Vec2& p)
        : pos(p), prevPos(p) {
    }

    CTransform(const Vec2& p, const Vec2& sc)
        : pos(p), prevPos(p), scale(sc) {
    }

    CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, sf::Angle a)
        : pos(p), prevPos(p), scale(sc), velocity(sp), angle(a) {
    }
};

struct CLifeSpan : Component {
    float lifespan = 0.0f;

    CLifeSpan() = default;

    CLifeSpan(const float duration)
        : lifespan(duration) {
    }
};

struct CDamage : Component {
    int damage = 1;

    CDamage() = default;

    explicit CDamage(const int d)
        : damage(d) {
    }
};

struct CInput : Component {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool attack = false;

    CInput() = default;
};

struct CBoundingBox : Component {
    Vec2 size;
    Vec2 halfSize;
    bool blockMove = false;
    bool blockVision = false;

    CBoundingBox() = default;

    explicit CBoundingBox(const Vec2& s)
        : size(s), halfSize(s.x / 2, s.y / 2) {
    }

    CBoundingBox(const Vec2& s, const bool bm, const bool bv)
        : size(s), halfSize(s.x / 2, s.y / 2), blockMove(bm), blockVision(bv) {
    }
};

struct CAnimation : Component {
    Animation animation;
    bool repeat = false;

    CAnimation() = default;

    CAnimation(const Animation& anim, const bool r)
        : animation(anim), repeat(r) {
    }
};

struct CState : Component {
    std::string state = "stand";
    bool changed = false;

    CState() = default;

    explicit CState(const std::string& s)
        : state(s) {
    }
};

struct CInvincibility : Component {
    float duration = 0.0f;

    CInvincibility() = default;

    explicit CInvincibility(const float d)
        : duration(d) {
    }
};

struct CHealth : Component {
    int max = 1;
    int current = 1;

    CHealth() = default;

    explicit CHealth(const int m)
        : max(m), current(m) {
    }
};

struct CPatrol : Component {
    std::vector<Vec2> positions;
    size_t currentPosition = 0;
    float speed = 0.0f;

    CPatrol() = default;

    CPatrol(const std::vector<Vec2>& pos, const float s)
        : positions(pos), speed(s) {
    }
};

struct CFollowPlayer : Component {
    Vec2 home;
    float speed = 0.0f;

    CFollowPlayer() = default;

    CFollowPlayer(const Vec2 p, const float s)
        : home(p), speed(s) {
    }
};
