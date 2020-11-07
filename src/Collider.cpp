#include "Collider.hpp"
#include "GameProperties.hpp"
#include "JamTemplate/Random.hpp"

Collider::Collider(std::shared_ptr<b2World> world, const Rect& rectDef)
    : Box2DObject(world, &GP::defaultColliderDef())
{
    m_collisionBox = std::make_shared<JamTemplate::SmartShape>();
    m_collisionBox->makeRect(rectDef.sizeDiagonal);
    m_collisionBox->setPosition(rectDef.position);
    m_collisionBox->setRotation(rectDef.rotation);
}

void Collider::toggleVisibility()
{
    if (m_visible) {
        m_collisionBox->setColor(sf::Color { 0, 0, 0, 0 });
    } else {
        m_collisionBox->setColor(JamTemplate::Random::getRandomColor());
    }
}
