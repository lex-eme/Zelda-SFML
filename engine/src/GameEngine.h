#pragma once

#include <unordered_map>
#include <string>

#include "Scene.h"
#include "Assets.h"

typedef std::unordered_map<std::string, std::shared_ptr<Scene> > SceneMap;

class GameEngine {
    sf::RenderWindow m_window;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    bool m_running = true;
    sf::Clock m_deltaClock;

public:
    static float s_deltaTime;

    explicit GameEngine(const std::string& path);

    void changeScene(const std::string& sceneName, const std::shared_ptr<Scene>& scene, bool endCurrentScene = false);

    void quit();
    void run();

    void playSound(const std::string& soundName);
    void stopSound(const std::string& soundName);

    sf::RenderWindow& window();
    const Assets& assets();
    bool isRunning() const;

private:
    void init(const std::string& path);
    void update();
    void sUserInput();
    std::shared_ptr<Scene> currentScene();
};
