#include "StateGame.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/SmartSprite.hpp"
#include "JamTemplate/SmartTilemap.hpp"
#include "JamTemplate/TweenAlpha.hpp"
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

    // TODO: Add object group for colliders here (m_colliders)
    m_colliders = std::make_shared<JamTemplate::ObjectGroup<Collider>>();

    m_tilemap = std::make_shared<JamTemplate::SmartTilemap>(
        std::filesystem::path("assets/tricky-tractor-level-0.json"));
    m_tilemap->setScreenSizeHint(GP::ScreenSizeInGame(), getGame());

    doCreateInternal();
    add(m_hud);

    m_world = std::make_shared<b2World>(b2Vec2 { 0, GP::GravityStrength() });

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2 { 100.0f, 100.0f };
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
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    m_tilemap->draw(getGame()->getRenderTarget());
    drawObjects();
    m_overlay->draw(getGame()->getRenderTarget());
}
