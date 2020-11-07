#ifndef TARGET_CONTACT_LISTENER_HPP_INCLUDEGUARD
#define TARGET_CONTACT_LISTENER_HPP_INCLUDEGUARD

#include "Box2D/Box2D.h"
#include "Target.hpp"

class TargetContactListener : public b2ContactListener {
public:
    TargetContactListener(std::function<void(float damage)> damageCallback);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

private:
    std::function<void(float damage)> m_callback;
};

#endif
