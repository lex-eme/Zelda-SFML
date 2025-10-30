#pragma once
#include "SFML/Graphics.hpp"

class Map final : public sf::Drawable {
    size_t m_width = 0;
    size_t m_heigh = 0;
    sf::VertexArray m_vertices = sf::VertexArray(sf::PrimitiveType::Triangles);
    const sf::Texture* m_texture = nullptr;
    float m_tileSize = 0.0f;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    Map() = default;
    Map(size_t width, size_t height, const sf::Texture* texture, float tileSize);

    void addTile(size_t index, const sf::Vector2f& tileWorldPos, const sf::FloatRect& tileUV);
    void removeTile(size_t index);
    void setVertexCount(size_t size);
    [[nodiscard]] size_t getVertexCount() const;

    [[nodiscard]] sf::Texture copyTexture() const;

    [[nodiscard]] size_t& width() { return m_width; }
    [[nodiscard]] size_t& height() { return m_heigh; }
    [[nodiscard]] float& tileSize() { return m_tileSize; }

    [[nodiscard]] const size_t& width() const { return m_width; }
    [[nodiscard]] const size_t& height() const { return m_heigh; }
    [[nodiscard]] const float& tileSize() const { return m_tileSize; }
};
