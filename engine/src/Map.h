#pragma once
#include "GameEngine.h"
#include "SFML/Graphics.hpp"

class MapLoader {
public:
    virtual ~MapLoader() = default;
    void virtual mapConfiguration(size_t width, size_t height) = 0;
    void virtual mapTile(int x, int y, size_t vertexIndex, size_t tileIndex) = 0;
};

class Map final : public sf::Drawable {
    GameEngine* m_game = nullptr;
    size_t m_width = 0;
    size_t m_height = 0;
    sf::VertexArray m_vertices = sf::VertexArray(sf::PrimitiveType::Triangles);
    sf::Transform m_transform;
    const sf::Texture* m_texture = nullptr;
    float m_tileSize = 0.0f;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    explicit Map(GameEngine* game);

    void loadFromFile(const std::string& path, MapLoader* loader = nullptr);
    void saveTexture(const std::string& path) const;

    void addTile(size_t index, const sf::Vector2f& pos, const sf::FloatRect& tileUV);
    void removeTile(size_t index);
    void setVertexCount(size_t size);
    [[nodiscard]] size_t getVertexCount() const;
    [[nodiscard]] size_t& width() { return m_width; }
    [[nodiscard]] size_t& height() { return m_height; }
    [[nodiscard]] float& tileSize() { return m_tileSize; }

    [[nodiscard]] const size_t& width() const { return m_width; }
    [[nodiscard]] const size_t& height() const { return m_height; }
    [[nodiscard]] const float& tileSize() const { return m_tileSize; }
};
