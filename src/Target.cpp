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
    m_animation->add("assets/legonit-break.png", "idle", sf::Vector2u { 24, 24 },
        JamTemplate::MathHelper::vectorBetween(0U, 0U), 0.1f);

    for (unsigned int i = 1; i <= GP::MaxCrystalDamage(); ++i) {
        registerDamageAnimation(i);
    }

    m_animation->play("idle", 0);
    m_animation->setOrigin(sf::Vector2f { 12, 12 });
    m_animation->setScale(sf::Vector2f { 0.5f, 0.5f });
    // create bounding box
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(6, 6);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;

    getB2Body()->CreateFixture(&fixtureDef);

    m_beamPosX = 100;

    m_beamShape = std::make_shared<JamTemplate::SmartSprite>();
    // m_beamShape->makeRect(sf::Vector2f { GP::TractorBeamWidth(), GP::ScreenSizeInGame().y });
    m_beamShape->loadSprite("assets/beam.png");
    m_beamShape->setColor(sf::Color { 255, 255, 255, 0 });
    m_beamShape->setScale(sf::Vector2f { 1.0f, GP::ScreenSizeInGame().y });

    m_beamBorderShape = std::make_shared<JamTemplate::SmartShape>();
    m_beamBorderShape->makeRect(sf::Vector2f { 1, GP::ScreenSizeInGame().y });

    m_glow = std::make_shared<JamTemplate::SmartSprite>();
    m_glow->loadSprite("#g#60#80");
    m_glow->setColor(GP::PaletteColorGlow());

    m_ufo = std::make_shared<JamTemplate::SmartAnimation>();
    m_ufo->add("assets/ufo.png", "idle", sf::Vector2u { 48, 20 },
        JamTemplate::MathHelper::vectorBetween(0U, 0U), 0.1f);
    m_ufo->play("idle");
    m_ufo->setOrigin(sf::Vector2f { m_ufo->getLocalBounds().width / 2, 0 });
    m_ufo->setOffset(sf::Vector2f { m_ufo->getLocalBounds().width / 2, 0 });

    /////////////////////////77
    // sounds

    m_sndBufTraktorLoop.loadFromFile("assets/sounds/traktor_brizzl_decline_1_loop.ogg");
    m_sndTraktorLoop.setBuffer(m_sndBufTraktorLoop);
    m_sndTraktorLoop.setLoop(true);
    m_sndTraktorLoop.setVolume(0);
    m_sndTraktorLoop.play();
}

void Target::doUpdate(float const elapsed)
{
    m_animation->setPosition(vec(getB2Body()->GetPosition()));
    m_animation->setRotation(JamTemplate::MathHelper::rad2deg(getB2Body()->GetAngle()));

    m_animation->update(elapsed);
    handleInput(elapsed);

    m_beamShape->setPosition(
        sf::Vector2f { m_beamPosX - GP::TractorBeamWidth() / 2, getGame()->getCamOffset().y });
    // m_beamShape->setPosition(m_animation->getPosition());
    m_beamShape->update(elapsed);
    m_beamBorderShape->update(elapsed);

    m_ufo->setPosition(sf::Vector2f { m_beamPosX - m_ufo->getLocalBounds().width / 2,
        static_cast<float>(static_cast<int>(5.0f * std::sin(getAge() * 3.5f))) });

    m_ufo->update(elapsed);

    auto vx = getB2Body()->GetLinearVelocity().x;
    auto vy = getB2Body()->GetLinearVelocity().y;
    vx *= GP::TargetAirFrictionX();
    getB2Body()->SetLinearVelocity(b2Vec2 { vx, vy });
    getB2Body()->SetAngularDamping(0.5f);
    m_glow->setPosition(m_animation->getPosition() - sf::Vector2f { 30, 30 });
    m_glow->update(elapsed);
}

void Target::handleInput(float const elapsed)
{
    m_beamPosXLast = m_beamPosX;
    m_beamPosX = JamTemplate::InputManager::getMousePositionWorld().x;
    float deltaX = m_beamPosX - m_beamPosXLast;
    // ufo tilting
    auto v = m_ufo->getRotation();

    double angleInc = -deltaX;
    if (v > 1) {
        v -= 25.f * elapsed;
    } else if (v < 1) {
        v += 25.f * elapsed;
    }
    v += angleInc;
    v = JamTemplate::MathHelper::clamp(v, -20.0f, 20.0f);

    m_ufo->setRotation(v);

    if (JamTemplate::InputManager::justPressed(sf::Mouse::Left)) {
        m_beamShape->flash(0.85f, sf::Color { 255, 255, 255, 40 });
        m_sndTraktorLoop.setVolume(25);
    } else if (JamTemplate::InputManager::justReleased(sf::Mouse::Left)) {

        m_sndTraktorLoop.setVolume(0);
    }
    if (JamTemplate::InputManager::pressed(sf::Mouse::Left)) {
        setBeamStrength(1.0f);

        using C = JamTemplate::Collision;
        if (C::BoundingBoxTest(m_animation, m_beamShape)) {
            // std::cout << "overlap\n";

            float deltaXClamped
                = JamTemplate::MathHelper::clamp(deltaX * GP::MouseMovementXToBeamConversion(),
                    -GP::TractorBeamStrengthX(), GP::TractorBeamStrengthX());

            float const forceY = m_vertivalBeamActive
                ? -GP::TractorBeamStrengthY() * getB2Body()->GetMass()
                : 0.0f;

            getB2Body()->ApplyForceToCenter(
                b2Vec2 { deltaXClamped * getB2Body()->GetMass(), forceY }, true);

        } else {
            // std::cout << "NO overlap\n";
        }
    } else {
        setBeamStrength(0.0f);
    }
}

void Target::doDraw() const
{
    m_animation->draw(getGame()->getRenderTarget());
    m_beamShape->draw(getGame()->getRenderTarget());

    m_beamBorderShape->setPosition(
        sf::Vector2f { m_beamPosX - GP::TractorBeamWidth() / 2, getGame()->getCamOffset().y });
    m_beamBorderShape->update(0.0f);
    m_beamBorderShape->draw(getGame()->getRenderTarget());

    m_beamBorderShape->setPosition(
        sf::Vector2f { m_beamPosX + GP::TractorBeamWidth() / 2, getGame()->getCamOffset().y });
    m_beamBorderShape->update(0.0f);
    m_beamBorderShape->draw(getGame()->getRenderTarget());
    m_glow->draw(getGame()->getRenderTarget());

    m_ufo->draw(getGame()->getRenderTarget());
}

void Target::setBeamStrength(float const v)
{
    float tval = (0.75f + 0.25f * (1 + std::sin(getAge() / 2.0f)) / 2.0f);
    // std::cout << tval << std::endl;
    sf::Uint8 v2 = static_cast<sf::Uint8>(
        tval * GP::BeamColorMaxAlpha() * JamTemplate::MathHelper::clamp(v, 0.0f, 1.0f));

    m_beamShape->setColor(sf::Color { 255, 255, 255, v2 });
}

void Target::registerDamageAnimation(unsigned int damage)
{
    std::stringstream animName;
    animName << "damaged" << damage;
    m_animation->add("assets/legonit-break.png", animName.str(), sf::Vector2u { 24, 24 },
        JamTemplate::MathHelper::vectorBetween(damage, damage), 0.1f);
}

void Target::setDamage(unsigned int currentDamage)
{
    m_damage = JamTemplate::MathHelper::clamp(currentDamage, 1U, GP::MaxCrystalDamage());
    std::stringstream animName;
    animName << "damaged" << m_damage;
    m_animation->play(animName.str(), 0);
}

unsigned int Target::getDamage() { return m_damage; }

sf::Vector2f Target::getTargetPosition() { return m_animation->getPosition(); }
