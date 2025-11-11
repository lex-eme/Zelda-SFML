#pragma once

#include <string>
#include <vector>

#include "Scene.h"

class Scene_Menu : public Scene {
    std::vector<std::string> m_menuStrings;
    std::vector<std::string> m_levelPaths;
    sf::Text m_menuText;
    size_t m_menuIndex = 0;

public:
    explicit Scene_Menu(GameEngine* gameEngine = nullptr);

private:
    void init();
    void update(float deltaTime) override;
    void onEnd() override;

    // Systems
    void sRender() override;
    void sDoAction(const Action& action) override;
};
