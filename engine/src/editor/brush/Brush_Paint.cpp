#include "Brush_Paint.h"

#include "editor/TileMapEditor.h"


namespace Engine {
    Brush_Paint::Brush_Paint(TileMapEditor* editor, sf::Sprite& sprite)
        : Brush(editor, sprite) {
    }

    void Brush_Paint::start(const sf::Vector2i pos) {
        if (m_started) {
            return;
        }
        m_started = true;
        m_editor->placeTile(pos);
    }

    void Brush_Paint::end(const sf::Vector2i pos) {
        m_started = false;
    }

    void Brush_Paint::update(const sf::Vector2i pos) {
        m_preview.setPosition({pos.x * m_editor->mapTileSize(), pos.y * m_editor->mapTileSize()});
    }

    void Brush_Paint::cancel() {
        m_started = false;
    }

    void Brush_Paint::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_preview);
    }
} // Engine
