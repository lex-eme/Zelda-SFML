#include "Assets.h"

#include <iostream>
#include <fstream>

void Assets::loadFromFile(const std::string& path) {
    std::ifstream fin(path);
    std::string type;

    while (fin >> type) {
        if (type == "Texture") {
            std::string name;
            std::string texturePath;
            fin >> name >> texturePath;
            addTexture(name, texturePath);
        } else if (type == "Animation") {
            std::string name;
            std::string textureName;
            size_t frameCount = 0, speed = 0;

            fin >> name >> textureName >> frameCount >> speed;

            const sf::Texture& texture = getTexture(textureName);
            Animation anim(name, texture, frameCount, speed);
            addAnimation(name, anim);
        } else if (type == "Font") {
            std::string name;
            std::string fontPath;

            fin >> name >> fontPath;

            addFont(name, fontPath);
        } else if (type == "Sound") {
            std::string name;
            std::string soundPath;

            fin >> name >> soundPath;

            addSound(name, soundPath);
        } else if (type == "SpriteSheet") {
            std::string name;
            std::string textureName;
            int width, height;

            fin >> name >> textureName >> width >> height;

            const sf::Texture& texture = getTexture(textureName);
            SpriteSheet spriteSheet(name, texture, width, height);
            addSpriteSheet(name, spriteSheet);
        }
    }
}

void Assets::addTexture(const std::string& name, const std::string& path) {
    if (m_textures.find(name) != m_textures.end()) {
        std::cout << "Texture '" << name << "' already exists" << std::endl;
        return;
    }

    if (!m_textures[name].loadFromFile(path)) {
        std::cerr << "Could not load texture '" << name << "' from path '" << path << "'" << std::endl;
        m_textures.erase(name);
    } else {
        std::cout << "Loaded Texture '" << name << "' from path '" << path << "'" << std::endl;
    }
}

void Assets::addAnimation(const std::string& name, const Animation& anim) {
    if (m_animations.find(name) != m_animations.end()) {
        std::cout << "Animation '" << name << "' already exists" << std::endl;
        return;
    }

    m_animations[name] = anim;
}

void Assets::addSpriteSheet(const std::string& name, const SpriteSheet& spriteSheet) {
    if (m_spriteSheets.find(name) != m_spriteSheets.end()) {
        std::cout << "Sprite sheet '" << name << "' already exists" << std::endl;
        return;
    }

    m_spriteSheets[name] = spriteSheet;
}

void Assets::addSound(const std::string& name, const std::string& path) {
    if (m_soundBuffers.find(name) != m_soundBuffers.end()) {
        std::cout << "SoundBuffer '" << name << "' already exists" << std::endl;
        return;
    }

    if (!m_soundBuffers[name].loadFromFile(path)) {
        std::cerr << "Could not load sound '" << name << "' from path '" << path << "'" << std::endl;
        m_soundBuffers.erase(name);
    } else {
        std::cout << "Loaded Sound '" << name << "' from path '" << path << "'" << std::endl;
        m_sounds.insert({name, sf::Sound(m_soundBuffers[name])});
        m_sounds.at(name).setVolume(25.0f);
    }
}

void Assets::addFont(const std::string& name, const std::string& path) {
    if (m_fonts.find(name) != m_fonts.end()) {
        std::cout << "Font '" << name << "' already exists" << std::endl;
        return;
    }

    if (!m_fonts[name].openFromFile(path)) {
        std::cerr << "Could not load font '" << name << "' from path '" << path << "'" << std::endl;
        m_fonts.erase(name);
    } else {
        std::cout << "Loaded Font '" << name << "' from path '" << path << "'" << std::endl;
    }
}

const sf::Texture& Assets::getTexture(const std::string& name) const {
    const auto it = m_textures.find(name);
    if (it != m_textures.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown texture called " << name << std::endl;
    exit(-1);
}

const Animation& Assets::getAnimation(const std::string& name) const {
    const auto it = m_animations.find(name);
    if (it != m_animations.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown animation called " << name << std::endl;
    exit(-1);
}

const SpriteSheet& Assets::getSpriteSheet(const std::string& name) const {
    const auto it = m_spriteSheets.find(name);
    if (it != m_spriteSheets.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown sprite sheet called " << name << std::endl;
    exit(-1);
}

sf::Sound& Assets::getSound(const std::string& name) {
    const auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown sound called " << name << std::endl;
    exit(-1);
}

const sf::Font& Assets::getFont(const std::string& name) const {
    const auto it = m_fonts.find(name);
    if (it != m_fonts.end()) {
        return it->second;
    }

    std::cerr << "Trying to use unknown font called " << name << std::endl;
    exit(-1);
}
