#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "Collider.hpp"
#include "JamTemplate/GameState.hpp"
#include "JamTemplate/ObjectGroup.hpp"
#include "Target.hpp"
#include <Box2D/Box2D.h>
#include <iostream>
#include <memory>

// fwd decls
namespace JamTemplate {
class SmartShape;
class SmartTilemap;
} // namespace JamTemplate

class Hud;

class StateGame : public JamTemplate::GameState {
public:
    StateGame(int levelID);

protected:
    std::shared_ptr<Hud> m_hud;

private:
    std::shared_ptr<JamTemplate::SmartShape> m_background;
    std::shared_ptr<JamTemplate::SmartShape> m_overlay;
    std::shared_ptr<Target> m_target;
    std::shared_ptr<JamTemplate::ObjectGroup<Collider>> m_colliders;
    std::shared_ptr<JamTemplate::SmartTilemap> m_tilemap;

    std::shared_ptr<b2World> m_world;

    std::shared_ptr<JamTemplate::SmartShape> m_endZone;

    int m_levelID;

    void doCreate() override;

    virtual void doCreateInternal();

    virtual void doInternalUpdate(float const elapsed) override;
    virtual void doInternalDraw() const override;

    void doScrolling(float const elapsed);
};

#endif
