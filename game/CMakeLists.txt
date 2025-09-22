set(SOURCES
        src/main.cpp
        src/Scene_Menu.cpp
        src/Scene_Menu.h
        src/Scene_Zelda.cpp
        src/Scene_Zelda.h)

add_executable(App)
target_sources(App PRIVATE ${SOURCES})

target_link_libraries(App PRIVATE Engine)
target_include_directories(App PRIVATE Source)