#include "Scene_Zelda.h"

#include "GameEngine.h"
#include "Physics.h"
#include "imgui.h"
#include "imgui-SFML.h"

#include <fstream>
#include <imgui_internal.h>

Scene_Zelda::Scene_Zelda(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_levelPath(levelPath), m_gridText(gameEngine->assets().getFont("Megaman")) {
    init(levelPath);
}

void Scene_Zelda::update(float deltaTime) {
    m_entityManager.update();

    if (!m_paused) {
        sAI();
        sMovement();
        sStatus();
        sCollision();
        sAnimation();
        sCamera();
    }
    sGUI();

    m_currentFrame += 1;
}

void Scene_Zelda::init(const std::string& levelPath) {
    loadLevel(levelPath);

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Mario"));

    registerAction(sf::Keyboard::Key::P, "PAUSE");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");
    registerAction(sf::Keyboard::Key::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::Key::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::Key::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::Key::F, "TOGGLE_FOLLOW");
    registerAction(sf::Keyboard::Key::Z, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::Q, "LEFT");
    registerAction(sf::Keyboard::Key::D, "RIGHT");
    registerAction(sf::Keyboard::Key::Space, "ATTACK");
}

void Scene_Zelda::loadLevel(const std::string& filename) {
    m_entityManager = EntityManager();

    std::ifstream fin(filename);
    std::string type;

    while (fin >> type) {
        if (type == "Tile") {
            std::string name;
            int RX, RY, TX, TY, BM, BV;
            fin >> name >> RX >> RY >> TX >> TY >> BM >> BV;

            auto e = m_entityManager.addEntity(type);

            auto& anim = m_game->assets().getAnimation(name);
            e->add<CAnimation>(anim, true);
            e->add<CTransform>(getPosition(RX, RY, TX, TY));
            e->add<CBoundingBox>(anim.getSize(), BM == 1, BV == 1);
        } else if (type == "NPC") {
            std::string name, AI;
            int RX, RY, TX, TY, BM, BV, H, D;
            fin >> name >> RX >> RY >> TX >> TY >> BM >> BV >> H >> D >> AI;

            auto e = m_entityManager.addEntity(type);

            auto& anim = m_game->assets().getAnimation(name);
            Vec2 pos = getPosition(RX, RY, TX, TY);
            e->add<CAnimation>(anim, true);
            e->add<CTransform>(pos);
            e->add<CBoundingBox>(anim.getSize(), BM == 1, BV == 1);
            e->add<CHealth>(H);
            e->add<CDamage>(D);

            if (AI == "Follow") {
                float s;
                fin >> s;
                e->add<CFollowPlayer>(pos, s);
            } else if (AI == "Patrol") {
                float s;
                int n;
                fin >> s >> n;

                std::vector<Vec2> positions;
                for (int i = 0; i < n; i++) {
                    int x, y;
                    fin >> x >> y;
                    positions.push_back(getPosition(RX, RY, x, y));
                }
                e->add<CPatrol>(positions, s);
            }
        } else if (type == "Player") {
            fin >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX
                    >> m_playerConfig.CY >> m_playerConfig.SPEED >> m_playerConfig.HEALTH;
        }
    }

    spawnPlayer();
}

Vec2 Scene_Zelda::getPosition(const int rx, const int ry, const int tx, const int ty) const {
    const float x = rx * static_cast<float>(width()) + tx * m_gridSize.x + m_gridSize.x / 2;
    const float y = ry * static_cast<float>(height()) + ty * m_gridSize.y + m_gridSize.y / 2;
    return {x, y};
}

void Scene_Zelda::spawnPlayer() {
    auto p = m_entityManager.addEntity("Player");
    p->add<CAnimation>(m_game->assets().getAnimation("StandDown"), true);
    p->add<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
    p->add<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY), true, false);
    p->add<CHealth>((int) m_playerConfig.HEALTH);
    p->add<CInput>();
    p->add<CState>();
}

static void placeSword(std::string& animName, CTransform& sTransform, const CTransform& eTransform) {
    if (eTransform.facing.x != 0.0f) {
        sTransform.pos.x = eTransform.pos.x + eTransform.facing.x * 60.0f;
        sTransform.pos.y = eTransform.pos.y;
        sTransform.scale.x = eTransform.facing.x;
        animName = "SwordRight";
    } else if (eTransform.facing.y != 0.0f) {
        sTransform.pos.y = eTransform.pos.y - eTransform.facing.y * 60.0f;
        sTransform.pos.x = eTransform.pos.x;
        sTransform.scale.y = eTransform.facing.y;
        animName = "SwordUp";
    }
}

static void direction(std::string& animName, CTransform& transform) {
    if (transform.facing.x != 0.0f) {
        animName.append("Right");
        transform.scale.x = transform.facing.x;
    } else if (transform.facing.y == -1.0f) {
        animName.append("Down");
        transform.scale.x = 1.0f;
    } else if (transform.facing.y == 1.0f) {
        animName.append("Up");
        transform.scale.x = 1.0f;
    }
}

void Scene_Zelda::sAnimation() {
    auto& pTransform = player()->get<CTransform>();
    auto& pState = player()->get<CState>();

    if (pState.changed) {
        std::string animName = pState.state;
        direction(animName, pTransform);
        player()->add<CAnimation>(m_game->assets().getAnimation(animName), true);
        pState.changed = false;
    }

    for (auto entity: m_entityManager.getEntities()) {
        if (entity->has<CAnimation>()) {
            auto& anim = entity->get<CAnimation>();
            anim.animation.update(GameEngine::s_deltaTime);

            if (!anim.repeat && anim.animation.hasEnded()) {
                entity->destroy();
            }
        }
    }
}

void Scene_Zelda::sCamera() {
    sf::View view = m_game->window().getView();
    auto& pos = player()->get<CTransform>().pos;

    if (m_follow) {
        view.setCenter({pos.x, pos.y});
    } else {
        const int w = static_cast<int>(width());
        const int h = static_cast<int>(height());

        int rx = static_cast<int>(pos.x) / w;
        if (pos.x < 0.0f) rx -= 1;

        int ry = static_cast<int>(pos.y) / h;
        if (pos.y < 0.0f) ry -= 1;

        view.setCenter({rx * w + w / 2.0f, ry * h + h / 2.0f});
    }

    m_game->window().setView(view);
}

void Scene_Zelda::moveEntities(const std::string& tag) {
    for (auto e: m_entityManager.getEntities(tag)) {
        auto& transform = e->get<CTransform>();
        transform.prevPos = transform.pos;
        transform.pos += transform.velocity * GameEngine::s_deltaTime;
    }
}

void Scene_Zelda::sMovement() {
    const auto& input = player()->get<CInput>();
    auto& transform = player()->get<CTransform>();
    auto& state = player()->get<CState>();

    const float speed = m_playerConfig.SPEED;
    Vec2 facing = transform.facing;
    bool run = true;
    if (input.up) {
        transform.velocity = Vec2(0.0f, -speed);
        facing = Vec2(0.0, 1.0f);
    } else if (input.down) {
        transform.velocity = Vec2(0.0f, speed);
        facing = Vec2(0.0, -1.0f);
    } else if (input.right) {
        transform.velocity = Vec2(speed, 0.0f);
        facing = Vec2(1.0, 0.0f);
    } else if (input.left) {
        transform.velocity = Vec2(-speed, 0.0f);
        facing = Vec2(-1.0, 0.0f);
    } else {
        transform.velocity = Vec2();
        run = false;
    }

    if (transform.facing != facing) {
        transform.facing = facing;
        state.changed = true;
    }
    if (input.attack) {
        if (state.state != "Atk") {
            state.state = "Atk";
            state.changed = true;
        }
    } else if (run) {
        if (state.state != "Run") {
            state.state = "Run";
            state.changed = true;
        }
    } else {
        if (state.state != "Stand") {
            state.state = "Stand";
            state.changed = true;
        }
    }

    moveEntities("Player");
    moveEntities("NPC");

    for (const auto sword: m_entityManager.getEntities("Sword")) {
        std::string animName;
        placeSword(animName, sword->get<CTransform>(), player()->get<CTransform>());
        sword->add<CAnimation>(m_game->assets().getAnimation(animName), true);
    }
}

void Scene_Zelda::spawnSword(std::shared_ptr<Entity> entity) {
    auto& input = entity->get<CInput>();
    if (input.attack)
        return;

    const auto sword = m_entityManager.addEntity("Sword");
    const auto& eTransform = entity->get<CTransform>();
    sword->add<CTransform>();
    std::string animName;
    placeSword(animName, sword->get<CTransform>(), eTransform);
    auto& anim = m_game->assets().getAnimation(animName);
    sword->add<CAnimation>(anim, true);
    sword->add<CBoundingBox>(anim.getSize());
    sword->add<CDamage>(1);
    sword->add<CLifeSpan>(0.1f);
    input.attack = true;
    m_game->playSound("Slash");
}

void Scene_Zelda::onEnd() {
    m_game->changeScene("MENU", nullptr, true);
}

std::shared_ptr<Entity> Scene_Zelda::player() {
    auto& entities = m_entityManager.getEntities("Player");
    if (entities.size() != 1) {
        std::cerr << "There is currently no player entity" << std::endl;
        exit(-1);
    }

    return entities[0];
}

void Scene_Zelda::sStatus() {
    for (auto sword: m_entityManager.getEntities("Sword")) {
        auto& lifespan = sword->get<CLifeSpan>();
        lifespan.lifespan -= GameEngine::s_deltaTime;

        if (lifespan.lifespan < 0.0f) {
            sword->destroy();
            player()->get<CInput>().attack = false;
        }
    }

    if (player()->has<CInvincibility>()) {
        auto& invincibility = player()->get<CInvincibility>();
        invincibility.duration -= GameEngine::s_deltaTime;

        if (invincibility.duration < 0.0f) {
            player()->remove<CInvincibility>();
        }
    }
}

void Scene_Zelda::sAI() {
    for (const auto enemy: m_entityManager.getEntities("NPC")) {
        if (enemy->has<CFollowPlayer>()) {
            auto& follow = enemy->get<CFollowPlayer>();
            auto& transform = enemy->get<CTransform>();
            auto& pPos = player()->get<CTransform>().pos;
            bool visionBlocked = false;

            for (const auto obstacle: m_entityManager.getEntities()) {
                if (!obstacle->has<CBoundingBox>()) continue;
                if (!obstacle->get<CBoundingBox>().blockVision) continue;

                visionBlocked |= Physics::EntityIntersect(transform.pos, pPos, obstacle);
            }

            if (visionBlocked) {
                if (follow.home.distSq(transform.pos) < 25.0f) {
                    transform.velocity = Vec2();
                    transform.pos = follow.home;
                } else {
                    transform.velocity = follow.home - transform.pos;
                }
            } else {
                transform.velocity = pPos - transform.pos;
            }
            transform.velocity.setMag(follow.speed);
        }

        if (enemy->has<CPatrol>()) {
            auto& patrol = enemy->get<CPatrol>();
            auto& transform = enemy->get<CTransform>();

            if (patrol.positions[patrol.currentPosition].distSq(transform.pos) < 25.0f) {
                patrol.currentPosition += 1;
                patrol.currentPosition %= patrol.positions.size();
            }

            transform.velocity = patrol.positions[patrol.currentPosition] - transform.pos;
            transform.velocity.setMag(patrol.speed);
        }
    }
}

static void resolveTileCollision(std::shared_ptr<Entity> tile, std::shared_ptr<Entity> entity) {
    const Vec2 overlap = Physics::GetOverlap(tile, entity);
    const Vec2 prevOverlap = Physics::GetPreviousOverlap(tile, entity);

    if (overlap.x >= 0.0f && overlap.y >= 0.0f) {
        const auto& tilePos = tile->get<CTransform>().pos;
        auto& entityPos = entity->get<CTransform>().pos;
        auto& entityPrevPos = entity->get<CTransform>().prevPos;

        if (prevOverlap.y > 0.0f) {
            entityPos.x += entityPos.x < tilePos.x ? -overlap.x : overlap.x;
        }

        if (prevOverlap.x > 0.0f) {
            entityPos.y += entityPos.y < tilePos.y ? -overlap.y : overlap.y;
        }
    }
}

void Scene_Zelda::tileCollision() {
    for (const auto tile: m_entityManager.getEntities("Tile")) {
        resolveTileCollision(tile, player());

        for (const auto npc: m_entityManager.getEntities("NPC")) {
            resolveTileCollision(tile, npc);
        }
    }
}

void Scene_Zelda::playerEnemyCollision() {
    auto p = player();

    if (p->has<CInvincibility>())
        return;

    for (const auto enemy: m_entityManager.getEntities("NPC")) {
        const Vec2 overlap = Physics::GetOverlap(enemy, p);
        if (overlap.x >= 0.0f && overlap.y >= 0.0f) {
            p->get<CHealth>().current -= enemy->get<CDamage>().damage;
            p->add<CInvincibility>(1.0f);

            if (p->get<CHealth>().current <= 0) {
                p->destroy();
                spawnPlayer();
                m_game->playSound("LinkDie");
            } else {
                m_game->playSound("LinkHurt");
            }
        }
    }
}

void Scene_Zelda::resolveHeartCollision(std::shared_ptr<Entity> heart, std::shared_ptr<Entity> entity) {
    const Vec2 overlap = Physics::GetOverlap(heart, entity);
    const Vec2 prevOverlap = Physics::GetPreviousOverlap(heart, entity);

    if (overlap.x >= 0.0f && overlap.y >= 0.0f) {
        entity->get<CHealth>().current = entity->get<CHealth>().max;
        heart->destroy();
        m_game->playSound("GetItem");
    }
}

void Scene_Zelda::heartCollision() {
    for (const auto tile: m_entityManager.getEntities("Tile")) {
        if (tile->get<CAnimation>().animation.getName() != "Heart")
            continue;

        resolveHeartCollision(tile, player());

        for (auto enemy: m_entityManager.getEntities("NPC")) {
            resolveHeartCollision(tile, enemy);
        }
    }
}

void Scene_Zelda::swordEnemyCollision() {
    for (const auto sword: m_entityManager.getEntities("Sword")) {
        if (!sword->has<CDamage>())
            continue;

        for (const auto enemy: m_entityManager.getEntities("NPC")) {
            const Vec2 overlap = Physics::GetOverlap(sword, enemy);

            if (overlap.x >= 0.0f && overlap.y >= 0.0f) {
                enemy->get<CHealth>().current -= sword->get<CDamage>().damage;
                sword->remove<CDamage>();

                if (enemy->get<CHealth>().current <= 0) {
                    enemy->destroy();
                    m_game->playSound("EnemyDie");
                } else {
                    m_game->playSound("EnemyHit");
                }
            }
        }
    }
}

void Scene_Zelda::sCollision() {
    tileCollision();
    playerEnemyCollision();
    swordEnemyCollision();
    heartCollision();
    // Implement black tile collisions / teleporting
}

static void entitiesTable(const EntityVec& entityVec) {
    if (ImGui::BeginTable("Tag Entities", 4,
                          ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuter |
                          ImGuiTableFlags_BordersInner)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Tag");
        ImGui::TableSetupColumn("Anim name");
        ImGui::TableSetupColumn("Position");
        ImGui::TableHeadersRow();

        for (const auto e: entityVec) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%zu", e->id());
            ImGui::TableNextColumn();
            ImGui::Text("%s", e->tag().c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", e->get<CAnimation>().animation.getName().c_str());
            ImGui::TableNextColumn();
            ImGui::Text("(%i,%i)", static_cast<int>(e->get<CTransform>().pos.x),
                        static_cast<int>(e->get<CTransform>().pos.y));
        }

        ImGui::EndTable();
    }
}

void Scene_Zelda::sGUI() {
    ImGui::Begin("Scene Properties");
    ImGui::GetCurrentContext();
    const ImGuiContext& g = *ImGui::GetCurrentContext();
    const ImGuiIO& io = g.IO;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    if (ImGui::BeginTabBar("MyTabBar")) {
        if (ImGui::BeginTabItem("Debug")) {
            ImGui::Checkbox("Draw Grid", &m_drawGrid);
            ImGui::Checkbox("Draw Textures", &m_drawTextures);
            ImGui::Checkbox("Draw Debug", &m_drawDebug);
            ImGui::Checkbox("Follow Cam", &m_follow);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Animations")) {
            // TODO
            ImGui::Text("Do this");

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Entity Manager")) {
            if (ImGui::TreeNode("All entities")) {
                entitiesTable(m_entityManager.getEntities());
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("By tag")) {
                for (auto& [tag, entityVec]: m_entityManager.getEntityMap()) {
                    if (ImGui::CollapsingHeader(tag.c_str())) {
                        entitiesTable(entityVec);
                    }
                }
                ImGui::TreePop();
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Scene_Zelda::sRender() {
    if (m_paused)
        m_game->window().clear(sf::Color(205, 142, 72));
    else
        m_game->window().clear(sf::Color(255, 192, 122));

    sf::RectangleShape tick({1.0f, 6.0f});
    tick.setFillColor(sf::Color::Black);

    if (m_drawTextures) {
        for (const auto e: m_entityManager.getEntities()) {
            auto& transform = e->get<CTransform>();
            sf::Color c = sf::Color::White;
            if (e->has<CInvincibility>()) {
                c = sf::Color(255, 255, 255, 128);
            }

            if (e->has<CAnimation>()) {
                auto& animation = e->get<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition({transform.pos.x, transform.pos.y});
                animation.getSprite().setScale({transform.scale.x, transform.scale.y});
                animation.getSprite().setColor(c);
                m_game->window().draw(animation.getSprite());
            }
        }

        // draw health bars
        for (const auto e: m_entityManager.getEntities()) {
            const auto& transform = e->get<CTransform>();
            if (e->has<CHealth>()) {
                const auto& h = e->get<CHealth>();
                Vec2 size(64, 6);
                sf::RectangleShape rect({size.x, size.y});
                rect.setPosition({transform.pos.x - 32, transform.pos.y - 48});
                rect.setFillColor(sf::Color(96, 96, 96));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(2.0f);
                m_game->window().draw(rect);

                const float ratio = (float) (h.current) / h.max;
                size.x *= ratio;
                rect.setSize({size.x, size.y});
                rect.setFillColor(sf::Color(255, 0, 0));
                rect.setOutlineThickness(0.0f);
                m_game->window().draw(rect);

                for (int i = 0; i < h.max; i++) {
                    tick.setPosition(rect.getPosition() + sf::Vector2f(i * 64.0f * 1 / h.max, 0.0f));
                    m_game->window().draw(tick);
                }
            }
        }
    }

    if (m_drawGrid) {
        const float leftX = m_game->window().getView().getCenter().x - width() / 2;
        const float rightX = leftX + width() + m_gridSize.x;
        const float topY = m_game->window().getView().getCenter().y - height() / 2;
        const float bottomY = topY + height() + m_gridSize.y;
        const float nextGridX = leftX - static_cast<int>(leftX) % static_cast<int>(m_gridSize.x);
        const float nextGridY = topY - static_cast<int>(topY) % static_cast<int>(m_gridSize.y);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
            drawLine(Vec2(x, topY), Vec2(x, bottomY));
        }

        for (float y = nextGridY; y < bottomY; y += m_gridSize.y) {
            drawLine(Vec2(leftX, y), Vec2(rightX, y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
                std::string xCell = std::to_string(static_cast<int>(x) / static_cast<int>(m_gridSize.x));
                std::string yCell = std::to_string(static_cast<int>(y) / static_cast<int>(m_gridSize.y));
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition({x + 3.0f, y + 2.0f});
                m_game->window().draw(m_gridText);
            }
        }
    }

    if (m_drawDebug) {
        sf::CircleShape dot(4.0f);
        dot.setFillColor(sf::Color::Black);
        for (auto e: m_entityManager.getEntities()) {
            if (e->has<CBoundingBox>()) {
                const auto& box = e->get<CBoundingBox>();
                auto& transform = e->get<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition({transform.pos.x, transform.pos.y});
                rect.setFillColor(sf::Color::Transparent);

                if (box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Black); } else if (
                    box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::Blue); } else if (
                    !box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Red); } else if (
                    !box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::White); }
                rect.setOutlineThickness(1.0f);
                m_game->window().draw(rect);
            }

            if (e->has<CPatrol>()) {
                auto& patrol = e->get<CPatrol>().positions;
                for (auto& p: patrol) {
                    dot.setPosition({p.x, p.y});
                    m_game->window().draw(dot);
                }
            }

            if (e->has<CFollowPlayer>()) {
                sf::VertexArray lines(sf::PrimitiveType::LineStrip, 2);
                lines[0].position.x = e->get<CTransform>().pos.x;
                lines[0].position.y = e->get<CTransform>().pos.y;
                lines[0].color = sf::Color::Black;
                lines[1].position.x = player()->get<CTransform>().pos.x;
                lines[1].position.y = player()->get<CTransform>().pos.y;
                lines[1].color = sf::Color::Black;
                m_game->window().draw(lines);
                dot.setPosition({e->get<CFollowPlayer>().home.x, e->get<CFollowPlayer>().home.y});
                m_game->window().draw(dot);
            }
        }
    }
}

void Scene_Zelda::sDoAction(const Action& action) {
    auto& input = player()->get<CInput>();

    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; } else if (
            action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; } else if (
            action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; } else if (
            action.name() == "TOGGLE_FOLLOW") { m_follow = !m_follow; } else if (
            action.name() == "PAUSE") { m_paused = !m_paused; } else if (action.name() == "QUIT") { onEnd(); } else if (
            action.name() == "UP") { input.up = true; } else if (action.name() == "DOWN") { input.down = true; } else if
        (action.name() == "LEFT") { input.left = true; } else if (action.name() == "RIGHT") { input.right = true; } else
            if (action.name() == "ATTACK") { spawnSword(player()); }
    } else if (action.type() == "END") {
        if (action.name() == "UP") { input.up = false; } else if (
            action.name() == "DOWN") { input.down = false; } else if (
            action.name() == "LEFT") { input.left = false; } else if (action.name() == "RIGHT") {
            input.right = false;
        }
    }
}
