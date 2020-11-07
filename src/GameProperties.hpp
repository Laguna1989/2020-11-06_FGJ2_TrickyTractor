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

    static sf::Color PaletteColor1() { return sf::Color { 38, 70, 83 }; }
    static sf::Color PaletteColor2() { return sf::Color { 42, 157, 143 }; }
    static sf::Color PaletteColor3() { return sf::Color { 233, 196, 106 }; }
    static sf::Color PaletteColor4() { return sf::Color { 244, 162, 97 }; }
    static sf::Color PaletteColor5() { return sf::Color { 231, 111, 81 }; }

    static sf::Color PaletteFlashGreen() { return sf::Color { 51, 143, 63 }; }
    static sf::Color PaletteFlashRed() { return sf::Color { 143, 33, 33 }; }

    static std::string GameName() { return "Tricky\nTractor"; }

    static const sf::Keyboard::Key KeyToggleDrawObjectGroups() { return sf::Keyboard::F5; };
    static const b2BodyDef defaultColliderDef(const Rect& rect)
    {
        b2BodyDef retVal {};
        retVal.position = JamTemplate::C::vec(rect.position);
        retVal.angle = JamTemplate::MathHelper::deg2rad(rect.rotation);
        return retVal;
    }
    static const std::string ColliderLayerName() { return "Terrain: Kollisionen"; }
    static const std::string OtherLayerName() { return "Terrain: Start"; }

    static float GravityStrength() { return 50.0f; }
    static float TractorBeamWidth() { return 48.0f; }
    static float TractorBeamStrengthY() { return 130.0f; }
    static float TractorBeamStrengthX() { return 180.0f; }
    static float MouseMovementXToBeamConversion() { return 70.0f; }
    static float TargetAirFrictionX() { return 0.992f; }
    static float AllowedCollisionSpeed() { return 8000.0f; }

    static sf::Vector2f WindowSizeOutGame() { return sf::Vector2f { 720, 900 }; }
    static float Zoom() { return 3.0f; }
    static sf::Vector2f ScreenSizeInGame() { return WindowSizeOutGame() / Zoom(); }
    static int TileSizeInPixel() { return 12; }

    static float ScrollBoundary() { return 87.0f; }
    static float ScrollSpeedX() { return 45.0f; }
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
            "assets/tricky-tractor-level-0.json", "Duplicate 1"));
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-0.json", "Duplicate 2"));
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-0.json", "Duplicat 3"));
        levelList.push_back(std::make_pair<std::string, std::string>(
            "assets/tricky-tractor-level-10.json", "Are you serious?"));
        return levelList;
    }

    static float GetMenuLevelTextDistance() { return 14; }
    static unsigned int MaxCrystalDamage()
    {
        return 4;
    } // Must have at least this many + 1 crystal frames
    static float InvulnerabilityAge() { return 2.0f; }
};

#endif
