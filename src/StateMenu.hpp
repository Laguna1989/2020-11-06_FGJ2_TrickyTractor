#ifndef GAME_STATE_MENU_HPP_INCLUDEGUARD
#define GAME_STATE_MENU_HPP_INCLUDEGUARD

#include "JamTemplate/GameState.hpp"
#include <SFML/Audio.hpp>

namespace JamTemplate {

class SmartText;
class SmartShape;
class SmartSprite;

} // namespace JamTemplate

class StateMenu : public JamTemplate::GameState {
public:
    using SmartText = JamTemplate::SmartText;
    StateMenu(float lastTime = 0.0f);

private:
    std::shared_ptr<JamTemplate::SmartShape> m_background;

    std::shared_ptr<SmartText> m_text_Title;
    std::shared_ptr<SmartText> m_text_Explanation;
    std::shared_ptr<SmartText> m_text_Explanation2;
    std::shared_ptr<SmartText> m_text_Credits;

    std::shared_ptr<JamTemplate::SmartShape> m_overlay;
    mutable std::shared_ptr<JamTemplate::SmartSprite> m_arrow;

    std::vector<std::shared_ptr<SmartText>> m_levelNames;
    unsigned int m_currentLevel;
    float m_inputDeadTimer;

    float m_lastTime;

    bool m_starting { false };

    bool m_grabCursor { true };

    void doInternalUpdate(float const /*elapsed*/) override;
    void doCreate() override;
    void doInternalDraw() const override;
};

#endif
