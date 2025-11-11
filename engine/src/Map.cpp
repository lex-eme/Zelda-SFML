#include "Map.h"

#include <fstream>
#include <iostream>

#include "SpriteSheet.h"

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = m_texture;
    states.transform *= m_transform;
    states.coordinateType = sf::CoordinateType::Pixels;
    target.draw(m_vertices, states);
}

Map::Map(GameEngine* game)
    : m_game(game) {
}

void Map::loadFromFile(const std::string& path, MapLoader* loader) {
    std::ifstream mapFin(path);
    std::string mapType;

    mapFin >> mapType;
    if (mapType != "Map") {
        std::cerr << "Expected 'Map', found '" << mapType << "'." << std::endl;
        return;
    }

    mapFin >> mapType;
    if (mapType != "c") {
        std::cerr << "Expected 'c', found '" << mapType << "'." << std::endl;
        return;
    }
    size_t tileCount;
    mapFin >> m_width >> m_height >> m_tileSize >> tileCount >> mapType;
    if (loader) {
        loader->mapConfiguration(m_width, m_height);
    }

    if (mapType != "n") {
        std::cerr << "Expected 'n', found '" << mapType << "'." << std::endl;
        return;
    }

    std::string name;
    mapFin >> name;
    const SpriteSheet& spriteSheet = m_game->assets().getSpriteSheet(name);
    m_texture = &spriteSheet.getTexture();
    m_vertices.resize(tileCount * 6);

    mapFin >> mapType;
    size_t index = 0;
    while (mapType == "t") {
        int x, y;
        size_t sheetIndex;
        mapFin >> x >> y >> sheetIndex;

        sf::Vector2i pos = sf::Vector2i(x, y);
        const auto uv = sf::FloatRect(spriteSheet.getTile(sheetIndex));
        addTile(index, pos, uv);
        mapFin >> mapType;
        if (loader) {
            loader->mapTile(x, y, index, sheetIndex);
        }
        index += 6;
    }

    if (mapType != "EndMap") {
        std::cerr << "Expected 'EndMap', found '" << mapType << "'." << std::endl;
        return;
    }

    std::cout << "Imported indices:" << index << std::endl;
}

void Map::saveTexture(const std::string& path) const {
    const auto size = sf::Vector2u(m_width * m_tileSize, m_height * m_tileSize);
    sf::RenderTexture rt(size);
    sf::RenderStates states;
    states.texture = m_texture;
    rt.draw(m_vertices, states);
    rt.display();
    if (rt.getTexture().copyToImage().saveToFile(path)) {
        std::cout << "Map exported to " << path << std::endl;
    }
}

void Map::addTile(const size_t index, const sf::Vector2i& pos, const sf::FloatRect& tileUV) {
    assert(index+6 <= m_vertices.getVertexCount());
    auto worldPos = sf::Vector2f(pos.x * m_tileSize, pos.y * m_tileSize);

    m_vertices[index + 0].position = {worldPos.x, worldPos.y};
    m_vertices[index + 1].position = {worldPos.x + m_tileSize, worldPos.y};
    m_vertices[index + 2].position = {worldPos.x, worldPos.y + m_tileSize};
    m_vertices[index + 3].position = {worldPos.x, worldPos.y + m_tileSize};
    m_vertices[index + 4].position = {worldPos.x + m_tileSize, worldPos.y + m_tileSize};
    m_vertices[index + 5].position = {worldPos.x + m_tileSize, worldPos.y};

    m_vertices[index + 0].texCoords = {tileUV.position.x, tileUV.position.y};
    m_vertices[index + 1].texCoords = {tileUV.position.x + tileUV.size.x, tileUV.position.y};
    m_vertices[index + 2].texCoords = {tileUV.position.x, tileUV.position.y + tileUV.size.y};
    m_vertices[index + 3].texCoords = {tileUV.position.x, tileUV.position.y + tileUV.size.y};
    m_vertices[index + 4].texCoords = {tileUV.position.x + tileUV.size.x, tileUV.position.y + tileUV.size.y};
    m_vertices[index + 5].texCoords = {tileUV.position.x + tileUV.size.x, tileUV.position.y};
}

void Map::removeTile(const size_t index) {
    m_vertices[index + 0].position = {0.0f, 0.0f};
    m_vertices[index + 1].position = {0.0f, 0.0f};
    m_vertices[index + 2].position = {0.0f, 0.0f};
    m_vertices[index + 3].position = {0.0f, 0.0f};
    m_vertices[index + 4].position = {0.0f, 0.0f};
    m_vertices[index + 5].position = {0.0f, 0.0f};
}

void Map::setVertexCount(const size_t size) {
    m_vertices.resize(size);
}

size_t Map::getVertexCount() const {
    return m_vertices.getVertexCount();
}
