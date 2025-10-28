#include "TileMapEditor.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "imgui-SFML.h"

TileMapEditor::TileMapEditor(GameEngine* gameEngine)
    : Scene(gameEngine), m_spriteSheet(m_game->assets().getSpriteSheet("Env1")),
      m_texture(m_spriteSheet.getTexture()), m_tilePreview(m_texture) {
    init();
}

void TileMapEditor::sRender() {
    auto& window = m_game->window();
    window.clear(sf::Color(150, 150, 150));

    sf::RenderStates states;
    states.texture = &m_texture;
    window.draw(m_map, states);
    window.draw(m_grid);
    if (m_selectedTile > 0) {
        const auto gridPos = getMouseGridPosition();

        if (!(gridPos.x < 0.0f || gridPos.x >= m_width || gridPos.y < 0.0f || gridPos.y >= m_height)) {
            m_tilePreview.setPosition(gridPos * m_tileSize);
            window.draw(m_tilePreview);
        }
    }

    sGUI();
}

void TileMapEditor::init() {
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerAction(sf::Keyboard::Key::A, "ZOOM IN");
    registerAction(sf::Keyboard::Key::E, "ZOOM OUT");
    registerAction(sf::Keyboard::Key::Z, "UP");
    registerAction(sf::Keyboard::Key::Q, "LEFT");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::D, "RIGHT");
    registerAction(sf::Keyboard::Key::Space, "PLACE");
    registerAction(sf::Keyboard::Key::X, "REMOVE");

    createGridVertexArray();

    m_tilePreview.setColor(sf::Color(255, 255, 255, 150));
    float scale = m_tileSize / m_spriteSheet.getTile(0).size.x;
    m_tilePreview.setScale({scale, scale});
    m_game->window().setVerticalSyncEnabled(true);
    m_indexVec.resize(m_width * m_height);
}

void TileMapEditor::update(float deltaTime) {
    m_entityManager.update();
}

void TileMapEditor::onEnd() {
    m_game->window().setVerticalSyncEnabled(false);
    m_game->changeScene("MENU", nullptr, true);
}

void TileMapEditor::sDoAction(const Action& action) {
    if (action.type() == "START") {
        if (action.name() == "QUIT") {
            if (m_selectedTile != 0) {
                m_selectedTile = 0;
            } else {
                onEnd();
            }
        } else if (action.name() == "ZOOM IN") {
            zoom(true);
        } else if (action.name() == "ZOOM OUT") {
            zoom(false);
        } else if (action.name() == "UP") {
            moveMap({0.0f, -1.0f});
        } else if (action.name() == "DOWN") {
            moveMap({0.0f, 1.0f});
        } else if (action.name() == "LEFT") {
            moveMap({-1.0f, 0.0f});
        } else if (action.name() == "RIGHT") {
            moveMap({1.0f, 0.0f});
        } else if (action.name() == "PLACE") {
            if (m_selectedTile != 0) {
                const auto pos = getMouseGridPosition();
                const auto gridPos = getMouseGridPosition();
                placeTile(pos, gridPos);
            }
        } else if (action.name() == "REMOVE") {
            auto pos = getMouseGridPosition();
            removeTile(pos);
        }
    }
}

void TileMapEditor::sGUI() {
    renderAssetBrowser();
}

void TileMapEditor::createGridVertexArray() {
    const size_t vertexCount = (m_width + 1) * 2 + (m_height + 1) * 2;
    m_grid = sf::VertexArray(sf::PrimitiveType::Lines, vertexCount);
    constexpr auto color = sf::Color::White;

    size_t index = 0;
    for (size_t col = 0; col <= m_width; col += 1) {
        m_grid[index].position = {col * m_tileSize, 0.0f};
        m_grid[index].color = color;
        m_grid[index + 1].position = {col * m_tileSize, m_height * m_tileSize};
        m_grid[index + 1].color = color;
        index += 2;
    }

    for (size_t row = 0; row <= m_height; row += 1) {
        m_grid[index].position = {0.0f, row * m_tileSize};
        m_grid[index].color = color;
        m_grid[index + 1].position = {m_width * m_tileSize, row * m_tileSize};
        m_grid[index + 1].color = color;
        index += 2;
    }
}

sf::Vector2f TileMapEditor::getMouseGridPosition() const {
    const auto& window = m_game->window();
    const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    const sf::Vector2f gridPos = sf::Vector2f(static_cast<int>(worldPos.x) / static_cast<int>(m_tileSize),
                                              static_cast<int>(worldPos.y) / static_cast<int>(m_tileSize));
    return gridPos;
}

sf::Vector2f TileMapEditor::getMouseWorldPosition() const {
    return getMouseGridPosition() * m_tileSize;
}

void TileMapEditor::placeTile(const sf::Vector2f& pos, const sf::Vector2f& gridPos) {
    size_t index;
    if (m_indexVec[gridPos.x + gridPos.y * m_width].isUsed) {
        std::cout << "Replace vertices." << std::endl;
        index = m_indexVec[gridPos.x + gridPos.y * m_width].index;
    } else {
        index = m_map.getVertexCount();
        m_map.resize(m_map.getVertexCount() + 6);
        m_indexVec[gridPos.x + gridPos.y * m_width] = {true, index};
    }

    const auto uv = sf::FloatRect(m_spriteSheet.getTile(m_selectedTile - 1));
    m_map[index + 0].position = {pos.x, pos.y};
    m_map[index + 1].position = {pos.x + m_tileSize, pos.y};
    m_map[index + 2].position = {pos.x, pos.y + m_tileSize};
    m_map[index + 3].position = {pos.x, pos.y + m_tileSize};
    m_map[index + 4].position = {pos.x + m_tileSize, pos.y + m_tileSize};
    m_map[index + 5].position = {pos.x + m_tileSize, pos.y};

    m_map[index + 0].texCoords = {uv.position.x, uv.position.y};
    m_map[index + 1].texCoords = {uv.position.x + uv.size.x, uv.position.y};
    m_map[index + 2].texCoords = {uv.position.x, uv.position.y + uv.size.y};
    m_map[index + 3].texCoords = {uv.position.x, uv.position.y + uv.size.y};
    m_map[index + 4].texCoords = {uv.position.x + uv.size.x, uv.position.y + uv.size.y};
    m_map[index + 5].texCoords = {uv.position.x + uv.size.x, uv.position.y};
}

void TileMapEditor::removeTile(const sf::Vector2f& pos) {
    if (!m_indexVec[pos.x + pos.y * m_width].isUsed) {
        return;
    }

    size_t index = m_indexVec[pos.x + pos.y * m_width].index;
    m_map[index + 0].position = {0.0f, 0.0f};
    m_map[index + 1].position = {0.0f, 0.0f};
    m_map[index + 2].position = {0.0f, 0.0f};
    m_map[index + 3].position = {0.0f, 0.0f};
    m_map[index + 4].position = {0.0f, 0.0f};
    m_map[index + 5].position = {0.0f, 0.0f};
    m_indexVec[pos.x + pos.y * m_width].isUsed = false;
}

void TileMapEditor::zoom(const bool in) const {
    auto view = m_game->window().getView();
    if (in) {
        view.zoom(1.0f / 0.8);
    } else {
        view.zoom(0.8);
    }
    m_game->window().setView(view);
}

void TileMapEditor::moveMap(const sf::Vector2f direction) const {
    auto view = m_game->window().getView();

    auto center = view.getCenter();
    center += direction * m_tileSize;
    view.setCenter(center);

    m_game->window().setView(view);
}

void TileMapEditor::renderAssetBrowser() {
    ImGui::Begin("Asset Browser");

    const ImGuiContext& g = *ImGui::GetCurrentContext();
    const ImGuiIO& io = g.IO;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("Entity count: %zu", m_entityManager.getEntities().size());

    ImGui::BeginChild("Map"); {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float scrollX = ImGui::GetScrollX();
        float scrollY = ImGui::GetScrollY();

        // Make sure the tilemap renders inside the child window
        drawList->PushClipRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y));

        int columnCount = 16;
        int tileSize = 32;
        constexpr ImVec4 clearColor(0.392f, 0.584f, 0.929f, 1.0f);
        ImU32 clearColorU32 = ImGui::ColorConvertFloat4ToU32(clearColor);
        SpriteSheet sheet = m_spriteSheet;
        size_t rowCount = sheet.size() / columnCount + (sheet.size() % columnCount == 0 ? 0 : 1);

        // Background color
        ImVec2 tilemapPos = ImVec2(windowPos.x - scrollX, windowPos.y - scrollY);
        ImVec2 tilemapSize = ImVec2(columnCount * tileSize, rowCount * tileSize);
        drawList->AddRectFilled(tilemapPos, {tilemapPos.x + tilemapSize.x, tilemapPos.y + tilemapSize.y},
                                clearColorU32);

        // Render tiles
        for (size_t i = 0; i < sheet.size(); i += 1) {
            size_t x = i % columnCount;
            size_t y = i / columnCount;
            ImVec2 pos = {windowPos.x + x * tileSize - scrollX, windowPos.y + y * tileSize - scrollY};
            ImVec2 size = ImVec2(pos.x + tileSize, pos.y + tileSize);

            auto textureHandle = reinterpret_cast<ImTextureID>(sheet.getNativeHandle());
            auto uv = sheet.getUV(i);
            drawList->AddImage(textureHandle, pos, size, uv.position, uv.position + uv.size);
        }

        // Draw grid
        {
            constexpr ImVec4 gridColor(0.678f, 0.678f, 0.678f, 1.0f);
            ImU32 gridColorU32 = ImGui::ColorConvertFloat4ToU32(gridColor);
            float gridThickness = 1.0f;

            // Vertical lines
            for (int x = 0; x <= columnCount; x += 1) {
                ImVec2 p1 = ImVec2(tilemapPos.x + x * tileSize, tilemapPos.y);
                ImVec2 p2 = ImVec2(tilemapPos.x + x * tileSize, tilemapPos.y + tilemapSize.y);
                drawList->AddLine(p1, p2, gridColorU32, gridThickness);
            }

            // Horizontal lines
            for (int y = 0; y <= rowCount; y += 1) {
                ImVec2 p1 = ImVec2(tilemapPos.x, tilemapPos.y + y * tileSize);
                ImVec2 p2 = ImVec2(tilemapPos.x + tilemapSize.x, tilemapPos.y + y * tileSize);
                drawList->AddLine(p1, p2, gridColorU32, gridThickness);
            }
        }

        ImGui::Dummy(ImVec2(tileSize * columnCount, tileSize * rowCount));

        // Convert cursor position to local coordinates within the tilemap
        int tileIndex = -1;
        ImVec2 cursorPos = ImGui::GetMousePos();
        ImVec2 localCursorPos = ImVec2(cursorPos.x - windowPos.x + scrollX, cursorPos.y - windowPos.y + scrollY);
        int tileIndexX = localCursorPos.x / tileSize;
        int tileIndexY = localCursorPos.y / tileSize;
        if (tileIndexX >= 0 && tileIndexY >= 0 && tileIndexX < columnCount && tileIndexY < rowCount) {
            tileIndex = tileIndexX + tileIndexY * columnCount;
        }

        if (tileIndex >= 0 && tileIndex < sheet.size() &&
            ImGui::IsMouseHoveringRect(tilemapPos, {tilemapPos.x + tilemapSize.x, tilemapPos.y + tilemapSize.y})) {
            ImVec2 topCorner(tileIndexX * tileSize + windowPos.x - scrollX,
                             tileIndexY * tileSize + windowPos.y - scrollY);
            ImVec2 bottomCorner(topCorner.x + tileSize, topCorner.y + tileSize);
            drawList->AddRect(
                topCorner, bottomCorner,
                IM_COL32(255, 255, 255, 255),
                0.0f,
                0,
                3.0f
            );

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                if (m_selectedTile == tileIndex + 1) {
                    m_selectedTile = 0;
                } else {
                    m_selectedTile = tileIndex + 1;
                    m_tilePreview.setTextureRect(m_spriteSheet.getTile(tileIndex));
                }

                std::cout << "Selected index: " << m_selectedTile << std::endl;
            }
        }

        drawList->PopClipRect();
    }
    ImGui::EndChild();

    ImGui::End();
}
