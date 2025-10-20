#include "Scene_Menu.h"

#include "GameEngine.h"
#include "Scene_Zelda.h"
#include "TileMapEditor.h"

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
    : Scene(gameEngine), m_menuText(gameEngine->assets().getFont("Megaman")) {
    init();
}

void Scene_Menu::sRender() {
    auto& window = m_game->window();
    window.clear(sf::Color::Blue);

    for (size_t i = 0; i < m_menuStrings.size(); i++) {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setPosition({30.0f, i * 60.0f + 40.0f});
        window.draw(m_menuText);
    }
}

void Scene_Menu::init() {
    registerAction(sf::Keyboard::Key::Z, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::D, "PLAY");
    registerAction(sf::Keyboard::Key::Q, "QUIT");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerAction(sf::Keyboard::Key::E, "EDITOR");

    m_menuStrings.emplace_back("Level  1");
    m_levelPaths.emplace_back("assets/levels/level1.txt");

    m_menuText.setFont(m_game->assets().getFont("Megaman"));
}

void Scene_Menu::update(float deltaTime) {
    m_entityManager.update();
    m_game->window().setView(m_game->window().getDefaultView());
}

void Scene_Menu::onEnd() {
    m_game->quit();
}

void Scene_Menu::sDoAction(const Action& action) {
    if (action.type() == "START") {
        if (action.name() == "UP") {
            if (m_menuIndex > 0) { m_menuIndex -= 1; } else { m_menuIndex = m_menuStrings.size() - 1; }
        } else if (action.name() == "DOWN") {
            m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size();
        } else if (action.name() == "PLAY") {
            m_game->changeScene("PLAY", std::make_shared<Scene_Zelda>(m_game, m_levelPaths[m_menuIndex]));
        } else if (action.name() == "QUIT") {
            onEnd();
        } else if (action.name() == "EDITOR") {
            m_game->changeScene("EDITOR", std::make_shared<TileMapEditor>(m_game));
        }
    }
}
