#pragma once

#include "Components.h"
#include "EntityMemoryPool.h"

class Entity {
    friend class EntityMemoryPool;

    size_t m_ID = 0;

    explicit Entity(size_t id);

public:
    void destroy() const;
    [[nodiscard]] size_t id() const;
    [[nodiscard]] const std::string& tag() const;
    [[nodiscard]] bool isActive() const;

    template<typename T>
    [[nodiscard]] bool has() const {
        return get<T>().has;
    }

    template<typename T, typename... TArgs>
    T& add(TArgs&&... mArgs) {
        auto& component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T& get() {
        return EntityMemoryPool::Instance().getComponent<T>(m_ID);
    }

    template<typename T>
    const T& get() const {
        return EntityMemoryPool::Instance().getComponent<T>(m_ID);
    }

    template<typename T>
    bool has() {
        return EntityMemoryPool::Instance().hasComponent<T>(m_ID);
    }

    template<typename T>
    void remove() {
        get<T>().has = false;;
    }
};
