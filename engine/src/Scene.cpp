#include "Scene.h"
#include "GameEngine.h"

Scene::Scene(GameEngine* gameEngine)
    : m_game(gameEngine) {
}

void Scene::simulate(const size_t frames) {
    for (size_t i = 0; i < frames; i++) {
        update(GameEngine::s_deltaTime);
    }
}

void Scene::registerAction(const sf::Keyboard::Key inputKey, const std::string& actionName) {
    m_actionMap[inputKey] = actionName;
}

size_t Scene::width() const {
    return m_game->window().getSize().x;
}

size_t Scene::height() const {
    return m_game->window().getSize().y;
}

size_t Scene::currentFrame() const {
    return m_currentFrame;
}

const ActionMap& Scene::getActionMap() const {
    return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2) const {
    const sf::Vertex line[] = {
        {{p1.x, p1.y}, sf::Color::Black, {0.0f, 0.0f}},
        {{p2.x, p2.y}, sf::Color::Black, {0.0f, 0.0f}}
    };

    m_game->window().draw(line, 2, sf::PrimitiveType::Lines);
}
