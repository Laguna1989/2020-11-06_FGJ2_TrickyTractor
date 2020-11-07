#include "Target.hpp"
#include "Collision.hpp"
#include "Conversions.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"

using namespace JamTemplate::C;

void Target::doCreate()
{
    m_animation = std::make_shared<JamTemplate::SmartAnimation>();

    m_animation->add("assets/coin.png", "idle", sf::Vector2u { 16, 16 },
        JamTemplate::MathHelper::vectorBetween(0U, 11U),
        JamTemplate::Random::getFloat(0.13f, 0.17f));
    m_animation->play("idle", JamTemplate::Random::getInt(0, 6));
    // m_animation->setOffset(sf::Vector2f { -8, -8 });
    m_animation->setOrigin(sf::Vector2f { 8, 8 });

    // create bounding box
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(8, 8);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;

    getB2Body()->CreateFixture(&fixtureDef);

    m_beamPosX = 100;

    m_beamShape = std::make_shared<JamTemplate::SmartShape>();
    m_beamShape->makeRect(sf::Vector2f { 32, GP::ScreenSizeInGame().y });
    m_beamShape->setColor(sf::Color { 255, 255, 255, 0 });

    m_beamBorderShape = std::make_shared<JamTemplate::SmartShape>();
    m_beamBorderShape->makeRect(sf::Vector2f { 1, GP::ScreenSizeInGame().y });
}

void Target::doUpdate(float const elapsed)
{
    m_animation->setPosition(vec(getB2Body()->GetPosition()));
    m_animation->setRotation(JamTemplate::MathHelper::rad2deg(getB2Body()->GetAngle()));

    m_animation->update(elapsed);
    handleInput(elapsed);

    m_beamShape->setPosition(sf::Vector2f { m_beamPosX, 0 });
    // m_beamShape->setPosition(m_animation->getPosition());
    m_beamShape->update(elapsed);
    m_beamBorderShape->update(elapsed);
}

void Target::handleInput(float const elapsed)
{
    if (JamTemplate::InputManager::pressed(sf::Keyboard::Key::Space)
        || JamTemplate::InputManager::pressed(sf::Keyboard::Key::W)
        || JamTemplate::InputManager::pressed(sf::Keyboard::Key::Up)
        || JamTemplate::InputManager::pressed(sf::Mouse::Left)) {
        setBeamStrength(1.0f);

        using C = JamTemplate::Collision;
        if (C::BoundingBoxTest(m_animation, m_beamShape)) {
            std::cout << "overlap\n";
            getB2Body()->ApplyForceToCenter(
                b2Vec2 { 0, -GP::TractorBeamStrengthY() * getB2Body()->GetMass() }, true);
        } else {
            std::cout << "NO overlap\n";
        }
    } else {

        setBeamStrength(0.0f);
    }

    m_beamPosX = JamTemplate::InputManager::getMousePositionWorld().x;
}

void Target::doDraw() const
{
    m_animation->draw(getGame()->getRenderTarget());
    m_beamShape->draw(getGame()->getRenderTarget());

    m_beamBorderShape->setPosition(sf::Vector2f { m_beamPosX, 0 });
    m_beamBorderShape->update(0.0f);
    m_beamBorderShape->draw(getGame()->getRenderTarget());

    m_beamBorderShape->setPosition(sf::Vector2f { m_beamPosX + 32, 0 });
    m_beamBorderShape->update(0.0f);
    m_beamBorderShape->draw(getGame()->getRenderTarget());
}

void Target::setBeamStrength(float const v)
{
    sf::Uint8 v2 = static_cast<sf::Uint8>(100.0f * JamTemplate::MathHelper::clamp(v, 0.0f, 1.0f));

    m_beamShape->setColor(sf::Color { 255, 255, 255, v2 });
}
