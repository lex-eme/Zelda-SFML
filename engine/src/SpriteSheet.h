#pragma once

#include <SFML/Graphics.hpp>

class SpriteSheet {
    sf::Sprite m_tileSheet;
    std::string m_name;
    std::vector<sf::FloatRect> m_tiles;

public:
    SpriteSheet();
    SpriteSheet(const std::string& name, const sf::Texture& texture, unsigned int tilesPerRow, unsigned int tilesPerColumn);

    size_t size() const;
    const sf::FloatRect& getUV(int index) const;
    size_t getNativeHandle() const;
};
