#pragma once

#include <vector>
#include <map>

#include "Entity.h"

using EntityVec = std::vector<Entity>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager {
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

public:
    EntityManager() = default;

    void update();

    Entity addEntity(const std::string& tag);

    const EntityVec& getEntities() const;
    const EntityVec& getEntities(const std::string& tag);
    const std::map<std::string, EntityVec>& getEntityMap();

private:
    static void removeDeadEntities(EntityVec& vec);
};
