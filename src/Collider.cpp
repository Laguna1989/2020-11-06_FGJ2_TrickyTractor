#include "Collider.hpp"
#include "GameProperties.hpp"
#include "MathHelper.hpp"

Collider::Collider(std::shared_ptr<b2World> world, const Rect& rectDef)
    : Box2DObject(world, &GP::defaultColliderDef(rectDef))
{

    b2Vec2 halfAxes(rectDef.sizeDiagonal.x / 2.0f, rectDef.sizeDiagonal.y / 2.0f);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(halfAxes.x, halfAxes.y, halfAxes, 0.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;
    fixtureDef.restitution = 0.7f;

    getB2Body()->CreateFixture(&fixtureDef);
}
