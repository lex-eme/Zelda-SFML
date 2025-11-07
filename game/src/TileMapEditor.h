#pragma once
#include "GameEngine.h"

#include "Map.h"
#include "Scene.h"

class TileMapEditor;

class Brush : public sf::Drawable {
protected:
    TileMapEditor* m_editor;

    explicit Brush(TileMapEditor* editor);

public:
    ~Brush() override = default;
    virtual void start(sf::Vector2f pos) = 0;
    virtual void end(sf::Vector2f pos) = 0;
    virtual void update(sf::Vector2f pos) = 0;
};

class Paint final : public Brush {
    bool m_started = false;

public:
    explicit Paint(TileMapEditor* editor);

    void start(sf::Vector2f pos) override;
    void end(sf::Vector2f pos) override;
    void update(sf::Vector2f pos) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class Rectangle final : public Brush {
    sf::Vector2f m_start;
    sf::Vector2f m_end;
    bool m_started = false;

public:
    explicit Rectangle(TileMapEditor* editor);

    void start(sf::Vector2f pos) override;
    void end(sf::Vector2f pos) override;
    void update(sf::Vector2f pos) override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class TileMapEditor final : public Scene {
    struct EditorMapEntry {
        bool isUsed = false;
        size_t vertexArrayIndex = 0;
        size_t spriteSheetIndex = 0;
    };

    Map m_mapClass;
    size_t m_selectedTile = 0;
    sf::VertexArray m_grid;
    std::vector<EditorMapEntry> m_editorMapEntries;
    size_t m_tileCount = 0;
    SpriteSheet m_spriteSheet;
    sf::Sprite m_tilePreview;
    Brush* m_brushes[2]{new Paint(this), new Rectangle(this)};
    size_t m_brushIndex = 0;
    bool m_showGrid = true;

public:
    explicit TileMapEditor(GameEngine* gameEngine = nullptr);
    ~TileMapEditor() override;

    void placeTile(const sf::Vector2f& pos);
    void placeTiles(const sf::Vector2f& from, const sf::Vector2f& to);
    float& mapTileSize() { return m_mapClass.tileSize(); }
    sf::Sprite& getPreviewSprite();

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
    void removeTile(const sf::Vector2f& pos);
    void zoom(bool in) const;
    void moveMap(sf::Vector2f direction) const;
    void renderAssetBrowser();

    size_t& mapWidth() { return m_mapClass.width(); }
    size_t& mapHeight() { return m_mapClass.height(); }

    [[nodiscard]] const size_t& mapWidth() const { return m_mapClass.width(); }
    [[nodiscard]] const size_t& mapHeight() const { return m_mapClass.height(); }
    [[nodiscard]] const float& mapTileSize() const { return m_mapClass.tileSize(); }
};
