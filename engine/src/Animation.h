#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Vec2.h"

class Animation {
    static sf::Texture s_defaultTexture;

    sf::Sprite m_sprite;
    size_t m_frameCount = 1;
    size_t m_currentFrame = 0;
    float m_frameDuration = 0.0f;
    float m_currentFrameDuration = 0.0f;
    Vec2 m_size;
    std::string m_name = "none";

public:
    Animation();
    Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed);

    void update(float deltaTime);
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2& getSize() const;
    sf::Sprite& getSprite();
};
