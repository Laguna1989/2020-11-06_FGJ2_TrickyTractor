#include "StateGame.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"
#include "SmartShape.hpp"
#include "SmartSprite.hpp"
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
    m_background->update(0.0f);

    m_overlay = std::make_shared<SmartShape>();
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
    if (!other.empty()) {
        startPosition = other.at(0).position;
        std::cout << startPosition.x << " " << startPosition.y << "\n";
    }
    getGame()->setCamOffset(startPosition);

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
    m_tilemap->update(elapsed);

    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    m_world->Step(elapsed, velocityIterations, positionIterations);

    doScrolling(elapsed);
}
void StateGame::doScrolling(float const elapsed)
{

    auto const mps = JamTemplate::InputManager::getMousePositionScreen();

    auto const mpsc
        = sf::Vector2f { JamTemplate::MathHelper::clamp(mps.x, 0.0f, GP::ScreenSizeInGame().x),
              JamTemplate::MathHelper::clamp(mps.y, 0.0f, GP::ScreenSizeInGame().y) };

    if (mpsc.x < GP::ScrollBoundary()) {
        getGame()->moveCam(sf::Vector2f { -GP::ScrllSpeed(), 0 } * elapsed);
    }
    if (mpsc.x > GP::ScreenSizeInGame().x - GP::ScrollBoundary()) {
        getGame()->moveCam(sf::Vector2f { GP::ScrllSpeed(), 0 } * elapsed);
    }

    if (mpsc.y < GP::ScrollBoundary()) {
        getGame()->moveCam(sf::Vector2f { 0, -GP::ScrllSpeed() } * elapsed);
    }
    if (mpsc.y > GP::ScreenSizeInGame().y - GP::ScrollBoundary()) {
        getGame()->moveCam(sf::Vector2f { 0, GP::ScrllSpeed() } * elapsed);
    }
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    m_tilemap->draw(getGame()->getRenderTarget());
    drawObjects();
    m_overlay->draw(getGame()->getRenderTarget());
}
