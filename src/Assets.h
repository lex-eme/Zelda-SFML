#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Animation.h"

class Assets {
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, Animation> m_animations;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Sound> m_sounds;
    std::unordered_map<std::string, sf::Font> m_fonts;

public:
    void loadFromFile(const std::string& path);

    void addTexture(const std::string& name, const std::string& path);
    void addAnimation(const std::string& name, const Animation& anim);
    void addSound(const std::string& name, const std::string& path);
    void addFont(const std::string& name, const std::string& path);

    const std::unordered_map<std::string, sf::Texture>& getTextures() const;
    const std::unordered_map<std::string, Animation>& getAnimations() const;
    const std::unordered_map<std::string, sf::Sound>& getSounds() const;

    const sf::Texture& getTexture(const std::string& name) const;
    const Animation& getAnimation(const std::string& name) const;
    sf::Sound& getSound(const std::string& name);
    const sf::Font& getFont(const std::string& name) const;
};
