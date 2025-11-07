#pragma once
#include "SFML/Graphics.hpp"

class TileMapEditor;

namespace Engine {
    class Brush : public sf::Drawable {
    protected:
        TileMapEditor* m_editor;

        explicit Brush(TileMapEditor* editor);

    public:
        ~Brush() override = default;
        virtual void start(sf::Vector2f pos) = 0;
        virtual void end(sf::Vector2f pos) = 0;
        virtual void update(sf::Vector2f pos) = 0;
        virtual void cancel() = 0;
    };
} // Engine
