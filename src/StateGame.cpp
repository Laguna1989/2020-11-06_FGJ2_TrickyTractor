#include "StateGame.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"
#include "SmartShape.hpp"
#include "SmartSprite.hpp"
#include "StateMenu.hpp"
#include "TweenAlpha.hpp"
#include <SmartTilemap.hpp>

void StateGame::doCreate()
{
    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    m_hud = std::make_shared<Hud>();

    using JamTemplate::SmartShape;
    using JamTemplate::TweenAlpha;

    m_background = std::make_shared<SmartShape>();
    m_background->makeRect({ w, h });
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    m_overlay = std::make_shared<SmartShape>();
    m_overlay->setIgnoreCamMovement(true);
    m_overlay->makeRect(sf::Vector2f { w, h });
    m_overlay->setColor(sf::Color { 0, 0, 0 });
    m_overlay->update(0);
    auto tw = TweenAlpha<SmartShape>::create(m_overlay, 0.5f, sf::Uint8 { 255 }, sf::Uint8 { 0 });
    tw->setSkipFrames();
    add(tw);

    m_tilemap = std::make_shared<JamTemplate::SmartTilemap>(
        std::filesystem::path("assets/tricky-tractor-level-0.json"));
    m_tilemap->setScreenSizeHint(GP::ScreenSizeInGame(), getGame());

    doCreateInternal();
    add(m_hud);

    m_world = std::make_shared<b2World>(b2Vec2 { 0, GP::GravityStrength() });

    m_colliders = std::make_shared<JamTemplate::ObjectGroup<Collider>>();
    auto const lol = m_tilemap->getObjectGroups().at(GP::ColliderLayerName());
    for (auto& rect : lol) {
        auto coll = std::make_shared<Collider>(m_world, rect);
        add(coll);
        m_colliders->push_back(coll);
    }
    add(m_colliders);

    // get start position
    sf::Vector2f startPosition;
    auto const other = m_tilemap->getObjectGroups().at(GP::OtherLayerName());
    for (auto const r : other) {
        if (r.m_type == "start") {
            startPosition = r.position;
            // std::cout << startPosition.x << " " << startPosition.y << "\n";
        } else if (r.m_type == "end") {
            m_endZone = std::make_shared<JamTemplate::SmartShape>();
            m_endZone->makeRect(r.sizeDiagonal);
            m_endZone->setColor(sf::Color { 255, 255, 255, 100 });
            m_endZone->setPosition(r.position);
            m_endZone->setRotation(r.rotation);
        }
    }
    getGame()->setCamOffset(startPosition - GP::ScreenSizeInGame() / 2.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = JamTemplate::C::vec(startPosition);
    m_target = std::make_shared<Target>(m_world, &bodyDef);
    add(m_target);
}

void StateGame::doCreateInternal() { }

void StateGame::doInternalUpdate(float const elapsed)
{
    m_overlay->update(elapsed);
    if (JamTemplate::InputManager::justReleased(GP::KeyToggleDrawObjectGroups())) {
        m_tilemap->toggleObjectGroupVisibility();
    }

    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    m_world->Step(elapsed, velocityIterations, positionIterations);

    if (m_endZone) {
        m_endZone->update(elapsed);
    }
    doScrolling(elapsed);
    if (JamTemplate::Collision::BoundingBoxTest(m_endZone, m_target->getTarget())) {
        getGame()->switchState(std::make_shared<StateMenu>());
    }
    m_tilemap->update(elapsed);

    // m_background->setPosition(getGame()->getCamOffset());
    // std::cout << getGame()->getCamOffset().x << " " << getGame()->getCamOffset().y << "\n";
    m_background->update(elapsed);
}
void StateGame::doScrolling(float const elapsed)
{

    auto const mps = JamTemplate::InputManager::getMousePositionScreen();
    auto const tpw = m_target->getTargetPosition();
    auto const tps = getGame()->getRenderWindow()->mapCoordsToPixel(tpw, *getGame()->getView())
        / static_cast<int>(GP::Zoom());

    auto const mpsc
        = sf::Vector2f { JamTemplate::MathHelper::clamp(mps.x, 0.0f, GP::ScreenSizeInGame().x),
              JamTemplate::MathHelper::clamp(mps.y, 0.0f, GP::ScreenSizeInGame().y) };

    if (mpsc.x < GP::ScrollBoundary()) {
        getGame()->moveCam(sf::Vector2f { -GP::ScrollSpeedX(), 0 } * elapsed);
    }
    if (mpsc.x > GP::ScreenSizeInGame().x - GP::ScrollBoundary()) {
        getGame()->moveCam(sf::Vector2f { GP::ScrollSpeedX(), 0 } * elapsed);
    }

    if (tps.y < GP::ScrollBoundary()) {
        float const f = (tps.y < 0) ? 2.0f : 1.0f;
        getGame()->moveCam(sf::Vector2f { 0, -GP::ScrollSpeedY() } * elapsed);
    }
    if (tps.y > GP::ScreenSizeInGame().y - GP::ScrollBoundary()) {
        float const f = (tps.y > GP::ScreenSizeInGame().y) ? 2.0f : 1.0f;

        getGame()->moveCam(sf::Vector2f { 0, GP::ScrollSpeedY() } * f * elapsed);
    }

    // limit cam movement to map
    auto const cpx = getGame()->getCamOffset().x;
    auto const cpy = getGame()->getCamOffset().y;

    auto const cw = GP::ScreenSizeInGame().x;
    auto const ch = GP::ScreenSizeInGame().y;

    auto const msxi = m_tilemap->getMapSizeInTiles().x;
    auto const msyi = m_tilemap->getMapSizeInTiles().y;
    auto const msx = static_cast<float>(msxi * GP::TileSizeInPixel());
    auto const msy = static_cast<float>(msyi * GP::TileSizeInPixel());

    auto cpxc = JamTemplate::MathHelper::clamp(cpx, 0.0f, msx - cw);
    auto cpyc = JamTemplate::MathHelper::clamp(cpy, 0.0f, msy - ch);

    // std::cout << cpx << " " << msx << " " << cpxc << "\n";

    getGame()->setCamOffset(sf::Vector2f { cpxc, cpyc });
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    m_tilemap->draw(getGame()->getRenderTarget());
    drawObjects();
    if (m_endZone) {
        m_endZone->draw(getGame()->getRenderTarget());
    }
    m_overlay->draw(getGame()->getRenderTarget());
}
