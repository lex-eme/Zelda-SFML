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
        src/GameEngine.cpp
        src/GameEngine.h
        src/Physics.cpp
        src/Physics.h
        src/Scene.cpp
        src/Scene.h)

add_library(Engine STATIC)
target_sources(Engine PRIVATE ${SOURCES})

target_link_libraries(Engine PUBLIC ImGui-SFML::ImGui-SFML SFML::Audio)

target_include_directories(Engine PUBLIC "src")