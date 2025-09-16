#include "Entity.h"

Entity::Entity(const size_t id, const std::string& tag)
    : m_ID(id), m_tag(tag),
      m_components(CTransform(), CLifeSpan(), CDamage(), CInput(), CBoundingBox(), CAnimation(), CState(),
                   CInvincibility(), CHealth(), CPatrol(), CFollowPlayer()) {
}

bool Entity::isActive() const {
    return m_active;
}

const std::string& Entity::tag() const {
    return m_tag;
}

const size_t Entity::id() const {
    return m_ID;
}

void Entity::destroy() {
    m_active = false;
}
