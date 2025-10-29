#pragma once
#include "GameEngine.h"

#include "Scene.h"

struct EditorMapEntry {
    bool isUsed = false;
    size_t vertexArrayIndex = 0;
    size_t spriteSheetIndex = 0;
};

class TileMapEditor : public Scene {
    size_t m_width = 64;
    size_t m_height = 64;
    size_t m_selectedTile = 0;
    sf::VertexArray m_grid;
    sf::VertexArray m_map = sf::VertexArray(sf::PrimitiveType::Triangles);
    std::vector<EditorMapEntry> m_editorMapEntries;
    size_t m_tileCount = 0;
    SpriteSheet m_spriteSheet;
    const sf::Texture m_texture;
    sf::Sprite m_tilePreview;
    float m_tileSize = 32.0f;
    bool m_showGrid = true;

public:
    explicit TileMapEditor(GameEngine* gameEngine = nullptr);

private:
    void init();
    void update(float deltaTime) override;
    void onEnd() override;

    void sRender() override;
    void sDoAction(const Action& action) override;
    void sGUI();

    void createGridVertexArray();
    void exportMap() const;
    void importMap();
    sf::Vector2f getMouseGridPosition() const;
    sf::Vector2f getMouseWorldPosition() const;
    void placeTile(const sf::Vector2f& pos, const sf::Vector2f& gridPos);
    void removeTile(const sf::Vector2f& pos);
    void zoom(bool in) const;
    void moveMap(sf::Vector2f direction) const;
    void renderAssetBrowser();
};
