#pragma once
#include "SFML/Graphics.hpp"

class TileMapEditor;

namespace Engine {
    class Brush : public sf::Drawable {
    protected:
        TileMapEditor* m_editor;
        sf::Sprite& m_preview;

        explicit Brush(TileMapEditor* editor, sf::Sprite& sprite);

    public:
        ~Brush() override = default;
        virtual void start(sf::Vector2i pos) = 0;
        virtual void end(sf::Vector2i pos) = 0;
        virtual void update(sf::Vector2i pos) = 0;
        virtual void cancel() = 0;
    };
} // Engine
