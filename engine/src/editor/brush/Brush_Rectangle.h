#pragma once
#include "Brush.h"

namespace Engine {
    class Brush_Rectangle final : public Brush {
        sf::Vector2f m_start;
        sf::Vector2f m_end;
        bool m_started = false;

    public:
        explicit Brush_Rectangle(TileMapEditor* editor);

        void start(sf::Vector2f pos) override;
        void end(sf::Vector2f pos) override;
        void update(sf::Vector2f pos) override;
        void cancel() override;

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
} // Engine
