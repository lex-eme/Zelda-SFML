#pragma once

#include <unordered_map>

#include "EntityManager.h"
#include "Action.h"

class GameEngine;

typedef std::unordered_map<sf::Keyboard::Key, std::string> ActionMap;

class Scene {
protected:
    GameEngine* m_game = nullptr;
    EntityManager m_entityManager;
    ActionMap m_actionMap;
    bool m_paused = false;
    size_t m_currentFrame = 0;

public:
    Scene() = default;
    explicit Scene(GameEngine* gameEngine);
    virtual ~Scene() = default;

    virtual void update(float deltaTime) = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    void simulate(size_t frames);
    void registerAction(sf::Keyboard::Key inputKey, const std::string& actionName);

    [[nodiscard]] size_t width() const;
    [[nodiscard]] size_t height() const;
    [[nodiscard]] size_t currentFrame() const;

    [[nodiscard]] bool hasEnded() const;

    [[nodiscard]] const ActionMap& getActionMap() const;
    void drawLine(const Vec2& p1, const Vec2& p2) const;

private:
    virtual void onEnd() = 0;
};
