#pragma once

#include <SFML/Graphics.hpp>

class SpriteSheet {
    sf::Sprite m_tileSheet;
    std::string m_name;
    std::vector<sf::FloatRect> m_UV;
    std::vector<sf::IntRect> m_tiles;

public:
    SpriteSheet();
    SpriteSheet(const std::string& name, const sf::Texture& texture, int tilesPerRow, int tilesPerColumn);

    size_t size() const;
    const sf::FloatRect& getUV(size_t index) const;
    const sf::IntRect& getTile(size_t index) const;
    size_t getNativeHandle() const;
};
