#include "Brush_Rectangle.h"

#include "editor/TileMapEditor.h"

namespace Engine {
    Brush_Rectangle::Brush_Rectangle(TileMapEditor* editor, sf::Sprite& sprite)
    : Brush(editor, sprite) {
    }

    void Brush_Rectangle::start(const sf::Vector2i pos) {
        if (m_started) {
            return;
        }
        m_started = true;
        m_start = pos;
        m_end = pos;
    }

    void Brush_Rectangle::end(const sf::Vector2i pos) {
        if (!m_started) {
            return;
        }
        m_started = false;
        const sf::Vector2i min = {std::min(m_start.x, m_end.x), std::min(m_start.y, m_end.y)};
        const sf::Vector2i max = {std::max(m_start.x, m_end.x), std::max(m_start.y, m_end.y)};

        for (int row = min.x; row <= max.x; row += 1) {
            for (int col = min.y; col <= max.y; col += 1) {
                auto tilePos = sf::Vector2i(row, col);
                m_editor->placeTile(tilePos);
            }
        }
    }

    void Brush_Rectangle::update(const sf::Vector2i pos) {
        m_end = pos;
    }

    void Brush_Rectangle::cancel() {
        m_started = false;
    }

    void Brush_Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!m_started) {
            m_preview.setPosition({m_end.x * m_editor->mapTileSize(), m_end.y * m_editor->mapTileSize()});
            target.draw(m_preview);
            return;
        }
        const sf::Vector2i min = {std::min(m_start.x, m_end.x), std::min(m_start.y, m_end.y)};
        const sf::Vector2i max = {std::max(m_start.x, m_end.x), std::max(m_start.y, m_end.y)};

        for (int row = min.x; row <= max.x; row += 1) {
            for (int col = min.y; col <= max.y; col += 1) {
                const auto pos = sf::Vector2f(row, col) * m_editor->mapTileSize();
                m_preview.setPosition(pos);
                target.draw(m_preview);
            }
        }
    }
} // Engine