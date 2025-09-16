add_executable(main main.cpp
        Action.cpp
        Action.h
        Vec2.cpp
        Vec2.h
        Animation.cpp
        Animation.h
        Assets.cpp
        Assets.h
        Components.h
        Entity.cpp
        Entity.h
        EntityManager.cpp
        EntityManager.h
        GameEngine.cpp
        GameEngine.h
        Physics.cpp
        Physics.h
        Scene.cpp
        Scene.h
        Scene_Menu.cpp
        Scene_Menu.h
        Scene_Zelda.cpp
        Scene_Zelda.h
)

target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML SFML::Audio)