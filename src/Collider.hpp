#ifndef COLLIDER_INCLUDEGUARD
#define COLLIDER_INCLUDEGUARD

#include "JamTemplate/Box2DObject.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "Rect.hpp"

class Collider : public JamTemplate::Box2DObject {
public:
    Collider(std::shared_ptr<b2World> world, const Rect& rectDef);

    void toggleVisibility();

private:
    std::shared_ptr<JamTemplate::SmartShape> m_collisionBox = nullptr;
    bool m_visible = false;
};

#endif
