set(SOURCES
        src/Action.cpp
        src/Action.h
        src/Vec2.cpp
        src/Vec2.h
        src/Animation.cpp
        src/Animation.h
        src/Assets.cpp
        src/Assets.h
        src/Components.h
        src/Entity.cpp
        src/Entity.h
        src/EntityManager.cpp
        src/EntityManager.h
        src/EntityMemoryPool.cpp
        src/EntityMemoryPool.h
        src/GameEngine.cpp
        src/GameEngine.h
        src/Physics.cpp
        src/Physics.h
        src/Scene.cpp
        src/Scene.h
        src/SpriteSheet.cpp
        src/SpriteSheet.h
        src/Map.cpp
        src/Map.h
        src/editor/brush/Brush.cpp
        src/editor/brush/Brush.h
        src/editor/TileMapEditor.cpp
        src/editor/TileMapEditor.h
        src/editor/brush/Brush_Paint.cpp
        src/editor/brush/Brush_Paint.h
        src/editor/brush/Brush_Rectangle.cpp
        src/editor/brush/Brush_Rectangle.h
)

add_library(Engine STATIC)
target_sources(Engine PRIVATE ${SOURCES})

target_link_libraries(Engine PUBLIC ImGui-SFML::ImGui-SFML SFML::Audio)

target_include_directories(Engine PUBLIC "src")