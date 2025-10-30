#include "Map.h"

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = m_texture;
    target.draw(m_vertices, states);
}

Map::Map(const size_t width, const size_t height, const sf::Texture* texture, const float tileSize)
    : m_width(width), m_heigh(height), m_texture(texture), m_tileSize(tileSize) {
}

void Map::addTile(const size_t index, const sf::Vector2f& tileWorldPos, const sf::FloatRect& tileUV) {
    m_vertices[index + 0].position = {tileWorldPos.x, tileWorldPos.y};
    m_vertices[index + 1].position = {tileWorldPos.x + m_tileSize, tileWorldPos.y};
    m_vertices[index + 2].position = {tileWorldPos.x, tileWorldPos.y + m_tileSize};
    m_vertices[index + 3].position = {tileWorldPos.x, tileWorldPos.y + m_tileSize};
    m_vertices[index + 4].position = {tileWorldPos.x + m_tileSize, tileWorldPos.y + m_tileSize};
    m_vertices[index + 5].position = {tileWorldPos.x + m_tileSize, tileWorldPos.y};

    m_vertices[index + 0].texCoords = {tileUV.position.x, tileUV.position.y};
    m_vertices[index + 1].texCoords = {tileUV.position.x + tileUV.size.x, tileUV.position.y};
    m_vertices[index + 2].texCoords = {tileUV.position.x, tileUV.position.y + tileUV.size.y};
    m_vertices[index + 3].texCoords = {tileUV.position.x, tileUV.position.y + tileUV.size.y};
    m_vertices[index + 4].texCoords = {tileUV.position.x + tileUV.size.x, tileUV.position.y + tileUV.size.y};
    m_vertices[index + 5].texCoords = {tileUV.position.x + tileUV.size.x, tileUV.position.y};
}

void Map::removeTile(const size_t index) {
    m_vertices[index + 0].position = {0.0f, 0.0f};
    m_vertices[index + 1].position = {0.0f, 0.0f};
    m_vertices[index + 2].position = {0.0f, 0.0f};
    m_vertices[index + 3].position = {0.0f, 0.0f};
    m_vertices[index + 4].position = {0.0f, 0.0f};
    m_vertices[index + 5].position = {0.0f, 0.0f};
}

void Map::setVertexCount(const size_t size) {
    m_vertices.resize(size);
}

size_t Map::getVertexCount() const {
    return m_vertices.getVertexCount();
}

sf::Texture Map::copyTexture() const {
    auto size = sf::Vector2u(m_width * m_tileSize, m_heigh * m_tileSize);
    sf::RenderTexture rt(size);
    sf::RenderStates states;
    states.texture = m_texture;
    rt.draw(m_vertices, states);
    rt.display();
    return rt.getTexture();
}
