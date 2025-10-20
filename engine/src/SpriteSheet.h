#pragma once

#include <SFML/Graphics.hpp>

class SpriteSheet {
    const sf::Texture& m_texture;
    std::string m_name;
    std::vector<sf::FloatRect> m_UV;
    std::vector<sf::IntRect> m_tiles;

public:
    SpriteSheet(const std::string& name, const sf::Texture& texture, int tilesPerRow, int tilesPerColumn);

    [[nodiscard]] size_t size() const;
    [[nodiscard]] const sf::FloatRect& getUV(size_t index) const;
    [[nodiscard]] const sf::IntRect& getTile(size_t index) const;
    [[nodiscard]] const sf::Texture& getTexture() const;
    [[nodiscard]] size_t getNativeHandle() const;
};
