#pragma once
#include "GameEngine.h"

#include "Map.h"
#include "Scene.h"

struct EditorMapEntry {
    bool isUsed = false;
    size_t vertexArrayIndex = 0;
    size_t spriteSheetIndex = 0;
};

class TileMapEditor : public Scene {
    Map m_mapClass;
    size_t m_selectedTile = 0;
    sf::VertexArray m_grid;
    std::vector<EditorMapEntry> m_editorMapEntries;
    size_t m_tileCount = 0;
    SpriteSheet m_spriteSheet;
    sf::Sprite m_tilePreview;
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

    size_t& mapWidth() { return m_mapClass.width(); }
    size_t& mapHeight() { return m_mapClass.height(); }
    float& mapTileSize() { return m_mapClass.tileSize(); }

    [[nodiscard]] const size_t& mapWidth() const { return m_mapClass.width(); }
    [[nodiscard]] const size_t& mapHeight() const { return m_mapClass.height(); }
    [[nodiscard]] const float& mapTileSize() const { return m_mapClass.tileSize(); }
};
