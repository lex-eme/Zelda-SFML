#include "Physics.h"

static Vec2 overlap(const Vec2& p1, const Vec2& p2, const Vec2& s1, const Vec2& s2) {
    const float overlapX = s2.x + s1.x - abs(p2.x - p1.x);
    const float overlapY = s2.y + s1.y - abs(p2.y - p1.y);

    return Vec2(overlapX, overlapY);
}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2) {
    const auto& p1 = e1->get<CTransform>().pos;
    const auto& p2 = e2->get<CTransform>().pos;
    const auto& bb1 = e1->get<CBoundingBox>().halfSize;
    const auto& bb2 = e2->get<CBoundingBox>().halfSize;
    return overlap(p1, p2, bb1, bb2);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2) {
    const auto& p1 = e1->get<CTransform>().prevPos;
    const auto& p2 = e2->get<CTransform>().prevPos;
    const auto& bb1 = e1->get<CBoundingBox>().halfSize;
    const auto& bb2 = e2->get<CBoundingBox>().halfSize;
    return overlap(p1, p2, bb1, bb2);
}

bool Physics::IsInside(const Vec2& pos, std::shared_ptr<Entity> e) {
    return false;
}

Intersect Physics::LineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d) {
    Vec2 r = b - a;
    Vec2 s = d - c;
    float rxs = r.cross(s);
    Vec2 cma = c - a;
    float t = cma.cross(s) / rxs;
    float u = cma.cross(r) / rxs;
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
        return {true, Vec2(a.x + t * r.x, a.y + t * r.y)};

    return {false, Vec2()};
}

bool Physics::EntityIntersect(const Vec2& a, const Vec2& b, std::shared_ptr<Entity> e) {
    bool intersects = false;
    auto& halfSize = e->get<CBoundingBox>().halfSize;
    auto& pos = e->get<CTransform>().pos;

    Vec2 tl(pos.x - halfSize.x, pos.y - halfSize.y);
    Vec2 tr(pos.x + halfSize.x, pos.y - halfSize.y);
    Vec2 bl(pos.x - halfSize.x, pos.y + halfSize.y);
    Vec2 br(pos.x + halfSize.x, pos.y + halfSize.y);

    intersects |= LineIntersect(a, b, tl, tr).result;
    intersects |= LineIntersect(a, b, tr, br).result;
    intersects |= LineIntersect(a, b, br, bl).result;
    intersects |= LineIntersect(a, b, bl, tl).result;

    return intersects;
}
