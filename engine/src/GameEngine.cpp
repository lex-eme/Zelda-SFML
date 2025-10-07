#include "GameEngine.h"

#include <iostream>
#include "imgui-SFML.h"

float GameEngine::s_deltaTime = 0.0f;

GameEngine::GameEngine(const std::string& path) {
    init(path);
}

void GameEngine::changeScene(const std::string& sceneName, const std::shared_ptr<Scene>& scene,
                             const bool endCurrentScene) {
    if (scene) {
        m_sceneMap[sceneName] = scene;
    } else {
        if (m_sceneMap.find(sceneName) == m_sceneMap.end()) {
            std::cerr << "Warning: Scene does not exist: " << sceneName << std::endl;
            return;
        }
    }

    if (endCurrentScene) {
        m_sceneMap.erase(m_sceneMap.find(m_currentScene));
    }

    m_currentScene = sceneName;
}

void GameEngine::quit() {
    m_running = false;
}

void GameEngine::run() {
    while (isRunning()) {
        update();
    }
    ImGui::SFML::Shutdown();
    m_window.close();
}

void GameEngine::playSound(const std::string& soundName) {
    m_assets.getSound(soundName).play();
}

void GameEngine::stopSound(const std::string& soundName) {
    m_assets.getSound(soundName).stop();
}

sf::RenderWindow& GameEngine::window() {
    return m_window;
}

const Assets& GameEngine::assets() {
    return m_assets;
}

bool GameEngine::isRunning() const {
    return m_running && m_window.isOpen();
}

void GameEngine::init(const std::string& path) {
    m_assets.loadFromFile(path);

    m_window.create(sf::VideoMode({1280, 768}), "Not Zelda");

    if (!ImGui::SFML::Init(m_window)) {
        std::cerr << "Could not init ImGui-SFML." << std::endl;
        exit(1);
    }

    EntityMemoryPool::Instance();
}

void GameEngine::update() {
    if (!isRunning() || m_sceneMap.empty())
        return;

    const sf::Time deltaTime = m_deltaClock.restart();
    s_deltaTime = deltaTime.asSeconds();
    ImGui::SFML::Update(m_window, deltaTime);

    sUserInput();
    currentScene()->simulate(1);
    currentScene()->sRender();
    ImGui::SFML::Render(m_window);
    m_window.display();
}

std::ostream& operator<<(std::ostream& lhs, const sf::Joystick::Axis& rhs) {
    switch (rhs) {
        case sf::Joystick::Axis::X: lhs << 'X';
            break;
        case sf::Joystick::Axis::Y: lhs << 'Y';
            break;
        case sf::Joystick::Axis::Z: lhs << 'Z';
            break;
        case sf::Joystick::Axis::R: lhs << 'R';
            break;
        case sf::Joystick::Axis::U: lhs << 'U';
            break;
        case sf::Joystick::Axis::V: lhs << 'V';
            break;
        case sf::Joystick::Axis::PovX: lhs << "PovX";
            break;
        case sf::Joystick::Axis::PovY: lhs << "PovY";
            break;
    }
    return lhs;
}

void GameEngine::sUserInput() {
    sf::Joystick::update();
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        ImGui::SFML::ProcessEvent(m_window, event.value());

        if (event->is<sf::Event::Closed>()) {
            quit();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::X) {
                std::cout << "trying to save screenshot to " << "test.png" << std::endl;
                sf::Texture texture(m_window.getSize());
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("test.png")) {
                    std::cout << "screenshot saved to " << "test.png" << std::endl;
                }
            }

            if (currentScene()->getActionMap().find(keyPressed->code) == currentScene()->getActionMap().end()) {
                continue;
            }

            currentScene()->sDoAction(Action(currentScene()->getActionMap().at(keyPressed->code), "START"));
        } else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (currentScene()->getActionMap().find(keyReleased->code) == currentScene()->getActionMap().end()) {
                continue;
            }

            currentScene()->sDoAction(Action(currentScene()->getActionMap().at(keyReleased->code), "END"));
        } else if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>()) {
            std::cout << "joystick button pressed!" << std::endl;
            std::cout << "joystick id: " << joystickButtonPressed->joystickId << std::endl;
            std::cout << "button: " << joystickButtonPressed->button << std::endl;
        } else if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>()) {
            std::cout << joystickMoved->axis << " axis moved!" << std::endl;
            std::cout << "joystick id: " << joystickMoved->joystickId << std::endl;
            std::cout << "new position: " << joystickMoved->position << std::endl;
        } else if (const auto* joystickConnected = event->getIf<sf::Event::JoystickConnected>()) {
            std::cout << "joystick connected: " << joystickConnected->joystickId << std::endl;
        } else if (const auto* joystickDisconnected = event->getIf<sf::Event::JoystickDisconnected>()) {
            std::cout << "joystick disconnected: " << joystickDisconnected->joystickId << std::endl;
        }
    }
}

std::shared_ptr<Scene> GameEngine::currentScene() {
    return m_sceneMap[m_currentScene];
}
