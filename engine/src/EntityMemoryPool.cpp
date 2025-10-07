#include "EntityMemoryPool.h"
#include "Entity.h"

constexpr size_t MAX_ENTITIES = 65536;

EntityMemoryPool::EntityMemoryPool(const size_t maxEntities) {
    std::get<std::vector<CTransform>>(m_pool).resize(maxEntities);
    std::get<std::vector<CPrevPosition>>(m_pool).resize(maxEntities);
    std::get<std::vector<CVelocity>>(m_pool).resize(maxEntities);
    std::get<std::vector<CFacing>>(m_pool).resize(maxEntities);
    std::get<std::vector<CLifeSpan>>(m_pool).resize(maxEntities);
    std::get<std::vector<CDamage>>(m_pool).resize(maxEntities);
    std::get<std::vector<CInput>>(m_pool).resize(maxEntities);
    std::get<std::vector<CBoundingBox>>(m_pool).resize(maxEntities);
    std::get<std::vector<CAnimation>>(m_pool).resize(maxEntities);
    std::get<std::vector<CState>>(m_pool).resize(maxEntities);
    std::get<std::vector<CInvincibility>>(m_pool).resize(maxEntities);
    std::get<std::vector<CHealth>>(m_pool).resize(maxEntities);
    std::get<std::vector<CPatrol>>(m_pool).resize(maxEntities);
    std::get<std::vector<CFollowPlayer>>(m_pool).resize(maxEntities);
    m_tags.resize(maxEntities);
    m_active.resize(maxEntities);
}

size_t EntityMemoryPool::getNextEntityIndex() const {
    for (size_t index = 0; index < MAX_ENTITIES; index++) {
        if (!m_active[index]) {
            return index;
        }
    }
    return 0;
}

EntityMemoryPool& EntityMemoryPool::Instance() {
    static EntityMemoryPool pool(MAX_ENTITIES);
    return pool;
}

const std::string& EntityMemoryPool::getTag(const size_t entityID) const {
    return m_tags[entityID];
}

bool EntityMemoryPool::getIsActive(const size_t entityID) const {
    return m_active[entityID];
}

Entity EntityMemoryPool::addEntity(const std::string& tag) {
    const size_t index = getNextEntityIndex();

    std::get<std::vector<CTransform>>(m_pool)[index] = CTransform();
    std::get<std::vector<CPrevPosition>>(m_pool)[index] = CPrevPosition();
    std::get<std::vector<CVelocity>>(m_pool)[index] = CVelocity();
    std::get<std::vector<CFacing>>(m_pool)[index] = CFacing();
    std::get<std::vector<CLifeSpan>>(m_pool)[index] = CLifeSpan();
    std::get<std::vector<CDamage>>(m_pool)[index] = CDamage();
    std::get<std::vector<CInput>>(m_pool)[index] = CInput();
    std::get<std::vector<CBoundingBox>>(m_pool)[index] = CBoundingBox();
    std::get<std::vector<CAnimation>>(m_pool)[index] = CAnimation();
    std::get<std::vector<CState>>(m_pool)[index] = CState();
    std::get<std::vector<CInvincibility>>(m_pool)[index] = CInvincibility();
    std::get<std::vector<CHealth>>(m_pool)[index] = CHealth();
    std::get<std::vector<CPatrol>>(m_pool)[index] = CPatrol();
    std::get<std::vector<CFollowPlayer>>(m_pool)[index] = CFollowPlayer();

    m_tags[index] = tag;
    m_active[index] = true;
    m_EntitiesCount += 1;

    return Entity(index);
}

void EntityMemoryPool::removeEntity(const size_t entityID) {
    m_EntitiesCount -= 1;
    m_active[entityID] = false;
}
