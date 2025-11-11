#include "SpriteSheet.h"

#include <iostream>

#include "Animation.h"
#include "GameEngine.h"

SpriteSheet::SpriteSheet(const std::string& name, const sf::Texture& texture, const int tilesPerRow,
                         const int tilesPerColumn)
    : m_texture(texture), m_name(name) {
    const sf::Vector2i texSize = sf::Vector2i(texture.getSize());
    const int tileWidth = texSize.x / tilesPerRow;
    const int tileHeight = texSize.y / tilesPerColumn;
    std::cout << "Tile width: " << texSize.x / tilesPerRow << ", height: " << texSize.y / tilesPerColumn << std::endl;

    m_UV.reserve(tilesPerRow * tilesPerColumn);
    m_tiles.reserve(tilesPerRow * tilesPerColumn);
    for (int y = 0; y < tilesPerColumn; y += 1) {
        for (int x = 0; x < tilesPerRow; x += 1) {
            m_UV.push_back(sf::FloatRect(
                {
                    static_cast<float>(x * tileWidth) / static_cast<float>(texSize.x),
                    static_cast<float>(y * tileHeight) / static_cast<float>(texSize.y)
                },
                {
                    static_cast<float>(tileWidth) / static_cast<float>(texSize.x),
                    static_cast<float>(tileHeight) / static_cast<float>(texSize.y)
                }));
            m_tiles.push_back(sf::IntRect({x * tileWidth, y * tileHeight}, {tileWidth, tileHeight}));
        }
    }
}

size_t SpriteSheet::size() const {
    return m_UV.size();
}

const std::string& SpriteSheet::getName() const {
    return m_name;
}

const sf::FloatRect& SpriteSheet::getUV(const size_t index) const {
    return m_UV.at(index);
}

const sf::IntRect& SpriteSheet::getTile(const size_t index) const {
    return m_tiles.at(index);
}

const sf::Texture& SpriteSheet::getTexture() const {
    return m_texture;
}

size_t SpriteSheet::getNativeHandle() const {
    return m_texture.getNativeHandle();
}
