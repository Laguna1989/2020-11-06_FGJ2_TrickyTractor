#ifndef GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD
#define GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD

#include "Conversions.hpp"
#include "MathHelper.hpp"
#include "Rect.hpp"
#include <Box2D/Dynamics/b2Body.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <utility>
#include <vector>

class GP {
public:
    GP() = delete;

    static sf::Color PaletteBackground() { return sf::Color { 20, 35, 58 }; }
    static sf::Color PaletteFontFront() { return sf::Color { 248, 249, 254 }; }
    static sf::Color PaletteFontShadow(sf::Uint8 a = 255) { return sf::Color { 26, 29, 36, a }; }
    static sf::Color PaletteSelector() { return sf::Color { 221, 228, 236 }; }
    static sf::Color PaletteHighlight() { return sf::Color { 163, 183, 205 }; }

    static sf::Color PaletteColor1() { return sf::Color { 213, 214, 219 }; }
    static sf::Color PaletteColor2() { return sf::Color { 42, 157, 143 }; }
    static sf::Color PaletteColor3() { return sf::Color { 233, 196, 106 }; }
    static sf::Color PaletteColor4() { return sf::Color { 242, 184, 136 }; }
    static sf::Color PaletteColor5() { return sf::Color { 181, 89, 69 }; }

    static sf::Color PaletteFlashGreen() { return sf::Color { 51, 143, 63 }; }
    static sf::Color PaletteFlashRed() { return sf::Color { 143, 33, 33 }; }
    static sf::Color PaletteColorGlow() { return sf::Color { 255, 255, 163 }; }

    static std::string GameName() { return "Tricky\nTractor"; }

    static sf::Keyboard::Key KeyToggleDrawObjectGroups() { return sf::Keyboard::F5; };

    static b2BodyDef* defaultColliderDef(const Rect& rect)
    {
        static b2BodyDef m_bodyDef;
        m_bodyDef.position = JamTemplate::C::vec(rect.position);
        m_bodyDef.angle = JamTemplate::MathHelper::deg2rad(rect.rotation);
        return &m_bodyDef;
    }
    static std::string ColliderLayerName() { return "Terrain: Kollisionen"; }
    static std::string OtherLayerName() { return "Terrain: Start"; }

    static float GravityStrength() { return 50.0f; }
    static float TractorBeamWidth() { return 48.0f; }
    static float TractorBeamStrengthY() { return 130.0f; }
    static float TractorBeamStrengthX() { return 180.0f; }
    static float MouseMovementXToBeamConversion() { return 70.0f; }
    static float TargetAirFrictionX() { return 0.992f; }
    static float AllowedCollisionSpeed() { return 7600.0f; }

    static sf::Vector2f WindowSizeOutGame() { return sf::Vector2f { 720, 900 }; }
    static float Zoom() { return 3.0f; }
    static sf::Vector2f ScreenSizeInGame() { return WindowSizeOutGame() / Zoom(); }
    static int TileSizeInPixel() { return 12; }

    static float ScrollBoundary() { return 87.0f; }
    static float ScrollSpeedX() { return 70.0f; }
    static float ScrollSpeedY() { return 70.0f; }

    static float StrongShakeDuration() { return 0.5f; }
    static float StrongShakeIntensity() { return 1.5f; }
    static float WeakShakeDuration() { return 0.3f; }
    static float WeakShakeIntensity() { return 0.4f; }

    static float StrongFlashDuration() { return 0.8f; }
    static sf::Color StrongFlashColor() { return sf::Color { 255, 250, 163, 128 }; }
    static float WeakFlashDuration() { return 0.3f; }
    static sf::Color WeakFlashColor() { return sf::Color { 225, 180, 35, 64 }; }
    static std::vector<std::pair<std::string, std::string>> getLevelList()
    {
        std::vector<std::pair<std::string, std::string>> levelList {};
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-0.json", "The Beginning"));
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-1.json", "Can't touch this!"));
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-2.json", "Welcome to the Jam!"));
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-10.json", "Are you serious?"));
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-3.json", "Down Under"));
        return levelList;
    }

    static float GetMenuLevelTextDistance() { return 14; }
    static unsigned int MaxCrystalDamage()
    {
        return 4;
    } // Must have at least this many + 1 crystal frames
    static float InvulnerabilityAge() { return 0.75f; }
    static float MenuInputDeadTime() { return 0.2f; }

    static float BeamColorMaxAlpha() { return 170.0f; }
};

#endif
