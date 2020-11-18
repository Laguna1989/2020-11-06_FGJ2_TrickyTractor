#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "Collider.hpp"
#include "JamTemplate/GameState.hpp"
#include "JamTemplate/ObjectGroup.hpp"
#include "ParticleSystem.hpp"
#include "Target.hpp"
#include "TargetContactListener.hpp"
#include <Box2D/Box2D.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>

// fwd decls
namespace JamTemplate {
class SmartShape;
class SmartTilemap;
class SmartText;
} // namespace JamTemplate

class Hud;

class StateGame : public JamTemplate::GameState {
public:
    StateGame(int levelID, float timer = 0.0f);

    std::shared_ptr<Hud> m_hud;
    static bool m_restartMusic;

private:
    std::shared_ptr<JamTemplate::SmartShape> m_background;
    std::shared_ptr<JamTemplate::SmartShape> m_overlay;
    std::shared_ptr<Target> m_target;
    std::shared_ptr<JamTemplate::ObjectGroup<Collider>> m_colliders;
    std::shared_ptr<JamTemplate::SmartTilemap> m_tilemap;
    std::shared_ptr<TargetContactListener> m_contactListener;
    std::shared_ptr<JamTemplate::SmartText> m_textTimer;
    float m_lastBeamParticle;

    std::shared_ptr<b2World> m_world;

    std::shared_ptr<JamTemplate::SmartShape> m_endZone;
    std::vector<std::shared_ptr<JamTemplate::SmartShape>> m_damagingZones;
    std::vector<std::shared_ptr<JamTemplate::SmartShape>> m_blockingZones;

    std::shared_ptr<JamTemplate::SmartSprite> m_vignette;

    std::shared_ptr<JamTemplate::ParticleSystem<JamTemplate::SmartSprite, 100>> m_particlesDust;
    std::shared_ptr<JamTemplate::ParticleSystem<JamTemplate::SmartShape, 100>> m_particlesBreak;
    std::shared_ptr<JamTemplate::ParticleSystem<JamTemplate::SmartShape, 100>> m_particlesBeam;

    int m_levelID;
    float m_lastCollisionAge = 0.0f;
    float m_isDead = false;
    bool m_alreadyTweening = false;

    float m_timer;
    float m_startTimer;

    void doCreate() override;

    virtual void doCreateInternal();

    virtual void doInternalUpdate(float const elapsed) override;
    virtual void doInternalDraw() const override;

    void doScrolling(float const elapsed);
    void handleDamage(float damage);
    void handleDeath(float const elapsed);

    bool playerIsInBlockingZone();

    void restartLevel();

    sf::SoundBuffer m_sndBufAlarm1;
    sf::SoundBuffer m_sndBufAlarm2;
    sf::SoundBuffer m_sndBufAlarm3;
    sf::SoundBuffer m_sndBufCollision;
    sf::Sound m_sndAlarm1;
    sf::Sound m_sndAlarm2;
    sf::Sound m_sndAlarm3;
    sf::Sound m_sndCollision;
};

#endif
