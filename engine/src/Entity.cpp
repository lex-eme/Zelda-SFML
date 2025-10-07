#include "Entity.h"

Entity::Entity(const size_t id)
    : m_ID(id) {
}

const std::string& Entity::tag() const {
    return EntityMemoryPool::Instance().getTag(m_ID);
}

bool Entity::isActive() const {
    return EntityMemoryPool::Instance().getIsActive(id());
}

size_t Entity::id() const {
    return m_ID;
}

void Entity::destroy() const {
    EntityMemoryPool::Instance().removeEntity(m_ID);
}
