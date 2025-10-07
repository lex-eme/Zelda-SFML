#pragma once

#include "Entity.h"

struct Intersect {
    bool result = false;
    Vec2 pos;
};

class Physics {
public:
    static Vec2 GetOverlap(Entity e1, Entity e2);
    static Vec2 GetPreviousOverlap(Entity e1, Entity e2);
    static bool IsInside(const Vec2& pos, Entity e);
    static Intersect LineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d);
    static bool EntityIntersect(const Vec2& a, const Vec2& b, Entity e);
};
