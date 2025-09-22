#include "Animation.h"

sf::Texture Animation::s_defaultTexture;

Animation::Animation()
    : m_sprite(s_defaultTexture) {
}

Animation::Animation(const std::string& name, const sf::Texture& t, const size_t frameCount, const size_t speed)
    : m_sprite(t),
      m_frameCount(frameCount),
      m_speed(speed),
      m_name(name) {
    m_size = Vec2(static_cast<float>(t.getSize().x / frameCount), static_cast<float>(t.getSize().y));
    m_sprite.setOrigin({m_size.x / 2.0f, m_size.y / 2.0f});
    m_sprite.setTextureRect({
        {static_cast<int>(std::floor(m_currentFrame * m_size.x)), 0},
        {static_cast<int>(m_size.x), static_cast<int>(m_size.y)}
    });
}

void Animation::update() {
    if (m_frameCount == 1)
        return;

    m_currentFrame += 1;
    size_t frame = m_currentFrame / m_speed;
    frame %= m_frameCount;

    m_sprite.setTextureRect({
        {static_cast<int>(frame * static_cast<int>(m_size.x)), 0},
        {static_cast<int>(m_size.x), static_cast<int>(m_size.y)}
    });
}

const Vec2& Animation::getSize() const {
    return m_size;
}

const std::string& Animation::getName() const {
    return m_name;
}

sf::Sprite& Animation::getSprite() {
    return m_sprite;
}

bool Animation::hasEnded() const {
    if (m_frameCount == 1) {
        return false;
    }
    const size_t frame = m_currentFrame / m_speed;
    return frame > m_frameCount;
}
