#include "TargetContactListener.hpp"
#include "GameProperties.hpp"
#include <iostream>

TargetContactListener::TargetContactListener(std::function<void(float damage)> damageCallback)
    : b2ContactListener()
    , m_callback(damageCallback)
{
}

void TargetContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    auto sumImpulse = impulse->normalImpulses[0] + impulse->tangentImpulses[0];
    if (impulse->count > 1) {
        sumImpulse += impulse->normalImpulses[1] + impulse->tangentImpulses[1];
    }
    m_callback(sumImpulse);
}
