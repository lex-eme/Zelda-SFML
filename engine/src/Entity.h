#pragma once

#include <string>
#include <tuple>

#include "Components.h"

class EntityManager;

using ComponentTuple = std::tuple<
    CTransform,
    CLifeSpan,
    CDamage,
    CInput,
    CBoundingBox,
    CAnimation,
    CState,
    CInvincibility,
    CHealth,
    CPatrol,
    CFollowPlayer
>;

class Entity {
    friend class EntityManager;

    bool m_active = true;
    size_t m_ID = 0;
    const std::string m_tag = "default";
    ComponentTuple m_components;

    Entity(size_t id, const std::string& tag);

public:
    void destroy();
    size_t id() const;
    bool isActive() const;
    const std::string& tag() const;

    template<typename T>
    bool has() const {
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
        return std::get<T>(m_components);
    }

    template<typename T>
    const T& get() const {
        return std::get<T>(m_components);
    }

    template<typename T>
    void remove() {
        get<T>() = T();
    }
};
