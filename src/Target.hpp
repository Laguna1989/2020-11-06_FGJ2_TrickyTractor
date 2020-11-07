#ifndef GAME_TARGET_HPP_INCLUDEGUARD
#define GAME_TARGET_HPP_INCLUDEGUARD

#include "JamTemplate/Box2DObject.hpp"
#include "JamTemplate/SmartAnimation.hpp"
#include <string>

class Target : public JamTemplate::Box2DObject {
public:
    Target(std::shared_ptr<b2World> world, const b2BodyDef* def)
        : Box2DObject { world, def }
    {
    }

private:
    std::shared_ptr<JamTemplate::SmartAnimation> m_animation;

    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doCreate() override;
};

#endif
