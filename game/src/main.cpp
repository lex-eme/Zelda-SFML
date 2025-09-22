#include "GameEngine.h"

#include "Scene_Menu.h"

int main() {
    GameEngine engine("assets/assets.txt");
    engine.changeScene("MENU", std::make_shared<Scene_Menu>(&engine));
    engine.run();
    return 0;
}
