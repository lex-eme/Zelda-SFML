#include "Animation.h"

sf::Texture Animation::s_defaultTexture;

Animation::Animation()
    : m_sprite(s_defaultTexture) {
}

Animation::Animation(const std::string& name, const sf::Texture& t, const size_t frameCount, const size_t speed)
    : m_sprite(t),
      m_frameCount(frameCount),
      m_frameDuration(1.0f/speed),
      m_name(name) {
    m_size = Vec2(static_cast<float>(t.getSize().x / frameCount), static_cast<float>(t.getSize().y));
    m_sprite.setOrigin({m_size.x / 2.0f, m_size.y / 2.0f});
    m_sprite.setTextureRect({
        {static_cast<int>(std::floor(m_currentFrame * m_size.x)), 0},
        {static_cast<int>(m_size.x), static_cast<int>(m_size.y)}
    });
}

void Animation::update(const float deltaTime) {
    if (m_frameCount == 1)
        return;

    m_currentFrameDuration += deltaTime;

    if (m_currentFrameDuration > m_frameDuration) {
        m_currentFrame += 1;
        m_currentFrameDuration = 0.0f;

        m_sprite.setTextureRect({
            {static_cast<int>(m_currentFrame % m_frameCount * static_cast<int>(m_size.x)), 0},
            {static_cast<int>(m_size.x), static_cast<int>(m_size.y)}
        });
    }
}

const Vec2& Animation::getSize() const {
    return m_size;
}

const std::string& Animation::getName() const {
    return m_name;
}

const sf::Sprite& Animation::getSprite() const {
    return m_sprite;
}

sf::Sprite& Animation::getSprite() {
    return m_sprite;
}

bool Animation::hasEnded() const {
    if (m_frameCount == 1) {
        return false;
    }
    return m_currentFrame > m_frameCount;
}
