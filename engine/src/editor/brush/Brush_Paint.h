#pragma once
#include "Brush.h"

namespace Engine {
    class Brush_Paint final : public Brush {
        bool m_started = false;

    public:
        explicit Brush_Paint(TileMapEditor* editor, sf::Sprite& sprite);

        void start(sf::Vector2i pos) override;
        void end(sf::Vector2i pos) override;
        void update(sf::Vector2i pos) override;
        void cancel() override;

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
} // Engine
