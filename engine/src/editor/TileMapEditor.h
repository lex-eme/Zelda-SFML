#pragma once
#include "GameEngine.h"

#include "Map.h"
#include "Scene.h"
#include "brush/Brush.h"
#include "brush/Brush_Paint.h"
#include "brush/Brush_Rectangle.h"

class TileMapEditor final : public Scene, public MapLoader {
    struct EditorMapEntry {
        bool isUsed = false;
        size_t vertexArrayIndex = 0;
        size_t spriteSheetIndex = 0;
    };

    Map m_map;
    size_t m_selectedTile = 0;
    sf::VertexArray m_grid;
    std::vector<EditorMapEntry> m_editorMapEntries;
    size_t m_tileCount = 0;
    SpriteSheet m_spriteSheet;
    sf::Sprite m_tilePreview;
    std::array<Engine::Brush*, 2> m_brushes = {new Engine::Brush_Paint(this), new Engine::Brush_Rectangle(this)};
    size_t m_brushIndex = 0;
    bool m_showGrid = true;

public:
    explicit TileMapEditor(GameEngine* gameEngine = nullptr);
    ~TileMapEditor() override;

    void placeTile(const sf::Vector2i& pos);
    float& mapTileSize() { return m_map.tileSize(); }
    sf::Sprite& getPreviewSprite();

    void mapConfiguration(size_t width, size_t height) override;
    void mapTile(int x, int y, size_t vertexIndex, size_t tileIndex) override;

private:
    void init();
    void update(float deltaTime) override;
    void onEnd() override;

    void sRender() override;
    void sDoAction(const Action& action) override;
    void sGUI();

    void createGridVertexArray();
    void exportMap() const;
    sf::Vector2i getMouseGridPosition() const;
    sf::Vector2f getMouseWorldPosition() const;
    void removeTile(const sf::Vector2i& pos);
    void zoom(bool in) const;
    void moveMap(sf::Vector2f direction) const;
    void renderAssetBrowser();

    size_t& mapWidth() { return m_map.width(); }
    size_t& mapHeight() { return m_map.height(); }

    [[nodiscard]] const size_t& mapWidth() const { return m_map.width(); }
    [[nodiscard]] const size_t& mapHeight() const { return m_map.height(); }
    [[nodiscard]] const float& mapTileSize() const { return m_map.tileSize(); }
};
