#include "EntityManager.h"

void EntityManager::update() {
    for (auto e: m_entitiesToAdd) {
        m_entities.push_back(e);
        m_entityMap[e.tag()].push_back(e);
    }

    m_entitiesToAdd.clear();
    removeDeadEntities(m_entities);

    for (auto& [tag, entityVec]: m_entityMap) {
        removeDeadEntities(entityVec);
    }
}

Entity EntityManager::addEntity(const std::string& tag) {
    const Entity entity = EntityMemoryPool::Instance().addEntity(tag);
    m_entitiesToAdd.push_back(entity);
    return entity;
}

const EntityVec& EntityManager::getEntities() const{
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag) {
    return m_entityMap[tag];
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap() {
    return m_entityMap;
}

void EntityManager::removeDeadEntities(EntityVec& vec) {
    auto it = vec.begin();
    while (it != vec.end()) {
        if (!it->isActive()) {
            it = vec.erase(it);
        } else {
            ++it;
        }
    }
}
