#pragma once

#include <string>

#include "Scene.h"
#include "Entity.h"
#include "Map.h"

class Scene_Zelda : public Scene {
    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, HEALTH;
    };

    PlayerConfig m_playerConfig{};
    Map m_map;
    sf::Text m_gridText;
    const Vec2 m_gridSize = {64.0f, 64.0f};
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    bool m_drawDebug = false;
    bool m_follow = false;

public:
    Scene_Zelda(GameEngine* gameEngine, const std::string& levelPath);

private:
    void init(const std::string& levelPath);
    void loadLevel(const std::string& filename);
    void onEnd() override;
    void update(float deltaTime) override;
    void spawnPlayer();
    void spawnSword(Entity entity);
    void spawnManyEntities();
    Vec2 getPosition(int rx, int ry, int tx, int ty) const;
    Entity player();

    // Systems
    void sMovement();
    void sAI();
    void sStatus();
    void sAnimation();
    void sCollision();
    void sCamera();
    void sGUI();
    void sRender() override;
    void sDoAction(const Action& action) override;

    // Helper
    void moveEntities(const std::string& tag);
    void tileCollision();
    void playerEnemyCollision();
    void heartCollision();
    void resolveHeartCollision(Entity heart, Entity entity);
    void swordEnemyCollision();
};
