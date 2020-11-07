#include "Target.hpp"
#include "Conversions.hpp"
#include "Game.hpp"
#include "MathHelper.hpp"

using namespace JamTemplate::C;

void Target::doCreate()
{
    m_animation = std::make_shared<JamTemplate::SmartAnimation>();

    m_animation->add("assets/coin.png", "idle", sf::Vector2u { 16, 16 },
        JamTemplate::MathHelper::vectorBetween(0U, 11U),
        JamTemplate::Random::getFloat(0.13f, 0.17f));
    m_animation->play("idle", JamTemplate::Random::getInt(0, 6));
    m_animation->setOffset(sf::Vector2f { -8, -8 });
    m_animation->setOrigin(sf::Vector2f { 8, 8 });

    // create bounding box
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(8, 8);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;

    getB2Body()->CreateFixture(&fixtureDef);
}

void Target::doUpdate(float const elapsed)
{

    m_animation->setPosition(vec(getB2Body()->GetPosition()));
    m_animation->setRotation(JamTemplate::MathHelper::rad2deg(getB2Body()->GetAngle()));

    m_animation->update(elapsed);

    std::cout << m_animation->getPosition().y << std::endl;

    if (getB2Body()->GetType() == b2BodyType::b2_dynamicBody) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            getB2Body()->ApplyForceToCenter(b2Vec2 { 1000, 0 }, true);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            getB2Body()->ApplyForceToCenter(b2Vec2 { -1000, 0 }, true);
        }
    }
}
void Target::doDraw() const { m_animation->draw(getGame()->getRenderTarget()); }
