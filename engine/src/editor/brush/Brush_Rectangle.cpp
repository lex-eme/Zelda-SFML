#include "Brush_Rectangle.h"

#include "editor/TileMapEditor.h"

namespace Engine {
    Brush_Rectangle::Brush_Rectangle(TileMapEditor* editor)
    : Brush(editor) {
    }

    void Brush_Rectangle::start(const sf::Vector2f pos) {
        if (m_started) {
            return;
        }
        m_started = true;
        m_start = pos;
        m_end = pos;
    }

    void Brush_Rectangle::end(const sf::Vector2f pos) {
        if (!m_started) {
            return;
        }
        m_started = false;
        const sf::Vector2f min = {std::min(m_start.x, m_end.x), std::min(m_start.y, m_end.y)};
        const sf::Vector2f max = {std::max(m_start.x, m_end.x), std::max(m_start.y, m_end.y)};

        for (float row = min.x; row <= max.x; row += 1.0f) {
            for (float col = min.y; col <= max.y; col += 1.0f) {
                auto pos = sf::Vector2f(row, col);
                m_editor->placeTile(pos);
            }
        }
    }

    void Brush_Rectangle::update(const sf::Vector2f pos) {
        m_end = pos;
    }

    void Brush_Rectangle::cancel() {
        m_started = false;
    }

    void Brush_Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (!m_started) {
            m_editor->getPreviewSprite().setPosition(m_end * m_editor->mapTileSize());
            target.draw(m_editor->getPreviewSprite());
            return;
        }
        const sf::Vector2f min = {std::min(m_start.x, m_end.x), std::min(m_start.y, m_end.y)};
        const sf::Vector2f max = {std::max(m_start.x, m_end.x), std::max(m_start.y, m_end.y)};

        for (float row = min.x; row <= max.x; row += 1.0f) {
            for (float col = min.y; col <= max.y; col += 1.0f) {
                const auto pos = sf::Vector2f(row, col) * m_editor->mapTileSize();
                m_editor->getPreviewSprite().setPosition(pos);
                target.draw(m_editor->getPreviewSprite());
            }
        }
    }
} // Engine