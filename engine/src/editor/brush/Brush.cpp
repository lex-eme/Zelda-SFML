#include "Brush.h"

namespace Engine {
    Brush::Brush(TileMapEditor* editor, sf::Sprite& sprite)
        : m_editor(editor), m_preview(sprite) {
    }
} // Engine
