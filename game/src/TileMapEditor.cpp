#include "TileMapEditor.h"

#include <fstream>
#include <imgui.h>
#include <imgui_internal.h>
#include "imgui-SFML.h"

TileMapEditor::TileMapEditor(GameEngine* gameEngine)
    : Scene(gameEngine), m_spriteSheet(m_game->assets().getSpriteSheet("Env1")),
      m_tilePreview(m_spriteSheet.getTexture()) {
    init();
}

void TileMapEditor::sRender() {
    auto& window = m_game->window();
    window.clear(sf::Color(150, 150, 150));

    if (m_showGrid) {
        window.draw(m_grid);
    }
    window.draw(m_mapClass);
    if (m_selectedTile > 0) {
        const auto gridPos = getMouseGridPosition();

        if (!(gridPos.x < 0.0f || gridPos.x >= mapWidth() || gridPos.y < 0.0f || gridPos.y >= mapHeight())) {
            m_tilePreview.setPosition(gridPos * mapTileSize());
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

    importMap();
    createGridVertexArray();

    m_tilePreview.setColor(sf::Color(255, 255, 255, 150));
    float scale = mapTileSize() / m_spriteSheet.getTile(0).size.x;
    m_tilePreview.setScale({scale, scale});
    m_game->window().setVerticalSyncEnabled(true);
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
                const auto pos = getMouseWorldPosition();
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
    ImGui::Begin("Asset Browser");
    const ImGuiContext& g = *ImGui::GetCurrentContext();
    const ImGuiIO& io = g.IO;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Checkbox("Draw Grid", &m_showGrid);
    if (ImGui::Button("Export")) {
        exportMap();
    }
    renderAssetBrowser();
    ImGui::End();
}

void TileMapEditor::createGridVertexArray() {
    const size_t vertexCount = (mapWidth() + 1) * 2 + (mapHeight() + 1) * 2;
    m_grid = sf::VertexArray(sf::PrimitiveType::Lines, vertexCount);
    constexpr auto color = sf::Color::White;

    size_t index = 0;
    for (size_t col = 0; col <= mapWidth(); col += 1) {
        m_grid[index].position = {col * mapTileSize(), 0.0f};
        m_grid[index].color = color;
        m_grid[index + 1].position = {col * mapTileSize(), mapHeight() * mapTileSize()};
        m_grid[index + 1].color = color;
        index += 2;
    }

    for (size_t row = 0; row <= mapHeight(); row += 1) {
        m_grid[index].position = {0.0f, row * mapTileSize()};
        m_grid[index].color = color;
        m_grid[index + 1].position = {mapWidth() * mapTileSize(), row * mapTileSize()};
        m_grid[index + 1].color = color;
        index += 2;
    }
}

void TileMapEditor::exportMap() const {
    std::ofstream file("./assets/map/mymap.txt", std::ios::out);

    file << "Map\n";
    file << "c " << mapWidth() << " " << mapHeight() << " " << mapTileSize() << " " << m_tileCount << "\n";
    file << "n " << m_spriteSheet.getName() << "\n";

    for (int i = 0; i < m_editorMapEntries.size(); i += 1) {
        if (m_editorMapEntries[i].isUsed) {
            file << "t " << i % mapWidth() << " " << i / mapWidth() << " " << m_editorMapEntries[i].spriteSheetIndex <<
                    "\n";
        }
    }
    file << "EndMap" << std::endl;
}

void TileMapEditor::importMap() {
    std::ifstream fin("./assets/map/mymap.txt");
    std::string type;

    fin >> type;
    if (type != "Map") {
        std::cerr << "Expected 'Map', found '" << type << "'." << std::endl;
        return;
    }

    size_t index = 0;
    fin >> type;
    if (type != "c") {
        std::cerr << "Expected 'c', found '" << type << "'." << std::endl;
        return;
    } {
        fin >> mapWidth() >> mapHeight() >> mapTileSize() >> m_tileCount >> type;
        if (type != "n") {
            std::cerr << "Expected 'n', found '" << type << "'." << std::endl;
            return;
        }
        std::string name;
        fin >> name;
        m_mapClass = Map(mapWidth(), mapHeight(), &m_spriteSheet.getTexture(), mapTileSize());
        m_mapClass.setVertexCount(m_tileCount * 6);
        m_editorMapEntries.resize(mapWidth() * mapHeight());
    }
    fin >> type;
    while (type == "t") {
        int x, y;
        size_t sheetIndex;
        fin >> x >> y >> sheetIndex;
        m_editorMapEntries[x + y * mapWidth()] = {true, index, sheetIndex};

        sf::Vector2f pos = sf::Vector2f(x * mapTileSize(), y * mapTileSize());
        const auto uv = sf::FloatRect(m_spriteSheet.getTile(sheetIndex));
        m_mapClass.addTile(index, pos, uv);
        index += 6;
        fin >> type;
    }

    if (type != "EndMap") {
        std::cerr << "Expected 'EndMap', found '" << type << "'." << std::endl;
        return;
    }

    std::cout << "Imported indices:" << index << std::endl;
}

sf::Vector2f TileMapEditor::getMouseGridPosition() const {
    const auto& window = m_game->window();
    const sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    const sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    const sf::Vector2f gridPos = sf::Vector2f(static_cast<int>(worldPos.x) / static_cast<int>(mapTileSize()),
                                              static_cast<int>(worldPos.y) / static_cast<int>(mapTileSize()));
    return gridPos;
}

sf::Vector2f TileMapEditor::getMouseWorldPosition() const {
    return getMouseGridPosition() * mapTileSize();
}

void TileMapEditor::placeTile(const sf::Vector2f& pos, const sf::Vector2f& gridPos) {
    size_t index;
    if (m_editorMapEntries[gridPos.x + gridPos.y * mapWidth()].isUsed) {
        std::cout << "Replace vertices." << std::endl;
        index = m_editorMapEntries[gridPos.x + gridPos.y * mapWidth()].vertexArrayIndex;
        m_editorMapEntries[gridPos.x + gridPos.y * mapWidth()].spriteSheetIndex = m_selectedTile - 1;
    } else {
        index = m_mapClass.getVertexCount();
        m_mapClass.setVertexCount(index + 6);
        m_editorMapEntries[gridPos.x + gridPos.y * mapWidth()] = {true, index, m_selectedTile - 1};
        m_tileCount += 1;
    }

    const auto uv = sf::FloatRect(m_spriteSheet.getTile(m_selectedTile - 1));
    m_mapClass.addTile(index, pos, uv);
}

void TileMapEditor::removeTile(const sf::Vector2f& pos) {
    if (!m_editorMapEntries[pos.x + pos.y * mapWidth()].isUsed) {
        return;
    }

    const size_t index = m_editorMapEntries[pos.x + pos.y * mapWidth()].vertexArrayIndex;
    m_mapClass.removeTile(index);
    m_editorMapEntries[pos.x + pos.y * mapWidth()].isUsed = false;
    m_tileCount -= 1;
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
    center += direction * mapTileSize();
    view.setCenter(center);

    m_game->window().setView(view);
}

void TileMapEditor::renderAssetBrowser() {
    ImGui::BeginChild("Sprite sheet"); {
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
}
