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
    Scene_Menu(GameEngine* gameEngine = nullptr);

    void sRender() override;

private:
    void init();
    void update() override;
    void onEnd() override;

    // Systems
    void sDoAction(const Action& action) override;
};
