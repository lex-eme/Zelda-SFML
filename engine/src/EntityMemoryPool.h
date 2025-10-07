#pragma once

#include <tuple>

#include "Components.h"

class Entity;

using EntityComponentVectorTuple = std::tuple<
    std::vector<CTransform>,
    std::vector<CPrevPosition>,
    std::vector<CVelocity>,
    std::vector<CFacing>,
    std::vector<CLifeSpan>,
    std::vector<CDamage>,
    std::vector<CInput>,
    std::vector<CBoundingBox>,
    std::vector<CAnimation>,
    std::vector<CState>,
    std::vector<CInvincibility>,
    std::vector<CHealth>,
    std::vector<CPatrol>,
    std::vector<CFollowPlayer>
>;

class EntityMemoryPool {
    size_t m_EntitiesCount = 0;
    EntityComponentVectorTuple m_pool;
    std::vector<std::string> m_tags;
    std::vector<bool> m_active;

    explicit EntityMemoryPool(size_t maxEntities);

    [[nodiscard]] size_t getNextEntityIndex() const;

public:
    static EntityMemoryPool& Instance();

    template<typename T>
    T& getComponent(size_t entityID) {
        return std::get<std::vector<T>>(m_pool)[entityID];
    }

    template<typename T>
    bool hasComponent(size_t entityID) {
        return std::get<std::vector<T>>(m_pool)[entityID].has;
    }

    [[nodiscard]] const std::string& getTag(size_t entityID) const;
    [[nodiscard]] bool getIsActive(size_t entityID) const;
    Entity addEntity(const std::string& tag);
    void removeEntity(size_t entityID);
};
