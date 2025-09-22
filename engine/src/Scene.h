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
    Scene();
    Scene(GameEngine* gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    void simulate(size_t frames);
    void registerAction(sf::Keyboard::Key inputKey, const std::string& actionName);

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;

    bool hasEnded() const;

    const ActionMap& getActionMap() const;
    void drawLine(const Vec2& p1, const Vec2& p2) const;

private:
    virtual void onEnd() = 0;
};
