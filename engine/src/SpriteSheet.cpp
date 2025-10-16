#include "SpriteSheet.h"

#include <iostream>

#include "Animation.h"
#include "GameEngine.h"

SpriteSheet::SpriteSheet()
    : m_tileSheet(GameEngine::s_defaultTexture) {
}

SpriteSheet::SpriteSheet(const std::string& name, const sf::Texture& texture, const unsigned int tilesPerRow,
                         const unsigned int tilesPerColumn)
    : m_tileSheet(texture), m_name(name) {
    const sf::Vector2u texSize = texture.getSize();
    const unsigned int tileWidth = texSize.x / tilesPerRow;
    const unsigned int tileHeight = texSize.y / tilesPerColumn;
    std::cout << "Tile width: " << texSize.x / tilesPerRow << ", height: " << texSize.y / tilesPerColumn << std::endl;

    m_tiles.reserve(tilesPerRow * tilesPerColumn);
    for (int y = 0; y < tilesPerColumn; y += 1) {
        for (int x = 0; x < tilesPerRow; x += 1) {
            m_tiles.push_back(sf::FloatRect(
                {
                    static_cast<float>(x * tileWidth) / static_cast<float>(texSize.x),
                    static_cast<float>(y * tileHeight) / static_cast<float>(texSize.y)
                },
                {
                    static_cast<float>(tileWidth) / static_cast<float>(texSize.x),
                    static_cast<float>(tileHeight) / static_cast<float>(texSize.y)
                }));
        }
    }
}

size_t SpriteSheet::size() const {
    return m_tiles.size();
}

const sf::FloatRect& SpriteSheet::getUV(const int index) const {
    return m_tiles.at(index);
}

size_t SpriteSheet::getNativeHandle() const {
    return m_tileSheet.getTexture().getNativeHandle();
}
