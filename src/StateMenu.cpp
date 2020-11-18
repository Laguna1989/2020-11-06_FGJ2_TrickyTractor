#include "StateMenu.hpp"
#include "GameProperties.hpp"
#include "HighScore.hpp"
#include "JamTemplate/Game.hpp"
#include "JamTemplate/InputManager.hpp"
#include "JamTemplate/MathHelper.hpp"
#include "JamTemplate/SmartShape.hpp"
#include "JamTemplate/SmartText.hpp"
#include "JamTemplate/TextureManager.hpp"
#include "JamTemplate/TweenAlpha.hpp"
#include "JamTemplate/TweenPosition.hpp"
#include "JamTemplate/TweenScale.hpp"
#include "StateGame.hpp"

StateMenu::StateMenu() { }

void StateMenu::doCreate()
{
    StateGame::m_restartMusic = false;
    float w = static_cast<float>(getGame()->getRenderTarget()->getSize().x);
    float h = static_cast<float>(getGame()->getRenderTarget()->getSize().y);
    float wC = w / 2;

    m_background = std::make_shared<JamTemplate::SmartShape>();
    m_background->makeRect({ w, h });
    m_background->setColor(GP::PaletteBackground());
    m_background->update(0.0f);

    m_text_Title = std::make_shared<JamTemplate::SmartText>();
    m_text_Title->loadFont("assets/font.ttf");
    m_text_Title->setCharacterSize(32U);
    m_text_Title->setText(GP::GameName());
    m_text_Title->SetTextAlign(JamTemplate::SmartText::TextAlign::CENTER);
    m_text_Title->setPosition({ wC, 5 });
    m_text_Title->setColor(GP::PaletteColor1());
    m_text_Title->update(0.0f);
    m_text_Title->setShadow(GP::PaletteFontShadow(), sf::Vector2f { 3, 3 });

    m_text_Explanation = std::make_shared<JamTemplate::SmartText>();
    m_text_Explanation->loadFont("assets/font.ttf");
    m_text_Explanation->setCharacterSize(16U);
    m_text_Explanation->setText("Press Space");
    m_text_Explanation->setPosition({ wC, 150 });
    m_text_Explanation->setColor(GP::PaletteColor4());
    m_text_Explanation->update(0.0f);
    auto textWidth = m_text_Explanation->getLocalBounds().width;
    auto const textHeight = m_text_Explanation->getLocalBounds().height;
    m_text_Explanation->setOrigin(sf::Vector2f { textWidth / 2, textHeight / 2 });
    m_text_Explanation->SetTextAlign(JamTemplate::SmartText::TextAlign::LEFT);
    m_text_Explanation->setShadow(GP::PaletteFontShadow(), sf::Vector2f { 3, 3 });

    m_text_Explanation2 = std::make_shared<JamTemplate::SmartText>();
    m_text_Explanation2->loadFont("assets/font.ttf");
    m_text_Explanation2->setCharacterSize(12U);

    m_text_Explanation2->setText("[L]ock Mouse: ON");
    m_text_Explanation2->setPosition({ wC, 200 });
    m_text_Explanation2->setColor(GP::PaletteColor5());
    m_text_Explanation2->update(0.0f);
    textWidth = m_text_Explanation2->getLocalBounds().width;
    m_text_Explanation2->setOrigin(sf::Vector2f { textWidth / 2, textHeight / 2 });
    m_text_Explanation2->SetTextAlign(JamTemplate::SmartText::TextAlign::LEFT);
    m_text_Explanation2->setShadow(GP::PaletteFontShadow(), sf::Vector2f { 3, 3 });

    m_text_Credits = std::make_shared<JamTemplate::SmartText>();
    m_text_Credits->loadFont("assets/font.ttf");
    m_text_Credits->setCharacterSize(8U);
    m_text_Credits->setText("Created by @xxBloodyOrange, @adkiem,\n@Synchronleuchter and "
                            "@Laguna_999\nfor #JamFranken2020\n2020-11-06");
    m_text_Credits->setPosition({ 4, 250 });
    m_text_Credits->setColor(GP::PaletteColor5());
    m_text_Credits->SetTextAlign(JamTemplate::SmartText::TextAlign::LEFT);
    m_text_Credits->update(0.0f);
    m_text_Credits->setShadow(GP::PaletteFontShadow(), sf::Vector2f { 1, 1 });

    m_overlay = std::make_shared<JamTemplate::SmartShape>();
    m_overlay->makeRect(sf::Vector2f { w, h });
    m_overlay->setColor(sf::Color { 0, 0, 0 });
    m_overlay->update(0);

    m_arrow = std::make_shared<JamTemplate::SmartSprite>();
    m_arrow->loadSprite("assets/arrow.png");

    using tp = JamTemplate::TweenPosition<JamTemplate::SmartText>;
    using ta = JamTemplate::TweenAlpha<JamTemplate::SmartText>;
    using ts = JamTemplate::TweenScale<JamTemplate::SmartText>;
    {
        auto tw = JamTemplate::TweenAlpha<JamTemplate::SmartShape>::create(
            m_overlay, 0.5f, sf::Uint8 { 255 }, sf::Uint8 { 0 });
        tw->setSkipFrames();
        add(tw);
    }

    {
        auto ta1 = ta::create(m_text_Title, 0.55f, 0, 255);
        ta1->setStartDelay(0.2f);
        ta1->setSkipFrames();
        add(ta1);
    }
    {
        auto s2 = m_text_Explanation->getPosition() + sf::Vector2f { -500, 0 };
        auto e2 = m_text_Explanation->getPosition();

        auto tw2 = tp::create(m_text_Explanation, 0.5f, s2, e2);
        tw2->setStartDelay(0.3f);
        tw2->setSkipFrames();

        tw2->addCompleteCallback([this]() {
            auto ts1 = ts::create(m_text_Explanation, 0.75f, sf::Vector2f { 1.0f, 1.0f },
                sf::Vector2f { 1.05f, 1.05f });
            ts1->setRepeat(true);
            ts1->setAgePercentConversion([](float age) {
                return JamTemplate::Lerp::cosine(
                    0.0f, 1.0f, JamTemplate::MathHelper::clamp(age, 0.0f, 1.0f));
            });
            add(ts1);
        });
        add(tw2);
    }

    {
        auto tax = ta::create(m_text_Explanation2, 0.55f, 0, 255);
        tax->setStartDelay(0.9f);
        tax->setSkipFrames();
        add(tax);
    }

    {
        auto s3 = m_text_Credits->getPosition() + sf::Vector2f { 0, 100 };
        auto e3 = m_text_Credits->getPosition();

        auto tw3 = tp::create(m_text_Credits, 0.35f, s3, e3);
        tw3->setStartDelay(0.8f);
        tw3->setSkipFrames();
        add(tw3);
    }

    m_currentLevel = 0;

    getGame()->getRenderWindow()->setMouseCursorGrabbed(true);

    std::vector<float> const scores = loadHighscores();

    if (scores.size() != GP::getLevelList().size()) {
        throw std::invalid_argument { "size of level list and highscore does not match" };
    }
    for (auto i = 0U; i != GP::getLevelList().size(); ++i) {
        auto st = std::make_shared<SmartText>();
        st->loadFont("assets/font.ttf");
        std::string scorestring
            = JamTemplate::MathHelper::floatToStringWithXDigits(scores.at(i), 2);
        st->setText(scorestring + " " + GP::getLevelList().at(i).second);
        st->setCharacterSize(12U);
        st->setPosition({ wC, static_cast<float>(100 + GP::GetMenuLevelTextDistance() * i) });
        auto col = GP::PaletteColor4();
        col.a = static_cast<uint8_t>(JamTemplate::MathHelper::clamp(
            255 - static_cast<int>(std::fabs(i - m_currentLevel)) * 200, 0, 255));
        st->setColor(col);
        st->update(0.0f);
        if (i == m_currentLevel) {
            st->setShadow(GP::PaletteFontShadow(), sf::Vector2f { 1, 1 });
        } else {
            st->setShadowActive(false);
        }
        m_levelNames.push_back(st);
    }

    m_inputDeadTimer = 0;

    getGame()->m_music.openFromFile("assets/sounds/song_menu.ogg");
    getGame()->m_music.setLoop(true);
    getGame()->m_music.play();
}

void StateMenu::doInternalUpdate(float const elapsed)
{
    if (!m_starting) {
        m_inputDeadTimer -= elapsed;

        using ip = JamTemplate::InputManager;
        if (ip::justPressed(sf::Keyboard::Key::Space)
            || ip::justPressed(sf::Keyboard::Key::Return)) {

            m_starting = true;
            auto tw = JamTemplate::TweenAlpha<JamTemplate::SmartShape>::create(
                m_overlay, 0.5f, sf::Uint8 { 0 }, sf::Uint8 { 255 });
            tw->setSkipFrames();
            tw->addCompleteCallback(
                [this]() { getGame()->switchState(std::make_shared<StateGame>(m_currentLevel)); });
            add(tw);
        }

        m_text_Title->update(elapsed);
        m_text_Explanation->update(elapsed);
        m_text_Explanation2->update(elapsed);

        if (m_inputDeadTimer <= 0) {

            if (JamTemplate::InputManager::justPressed(sf::Keyboard::L)) {
                m_grabCursor = !m_grabCursor;
                getGame()->getRenderWindow()->setMouseCursorGrabbed(m_grabCursor);
                std::string const str = (m_grabCursor ? "ON" : "OFF");
                m_text_Explanation2->setText("[L]ock Mouse: " + str);
                m_inputDeadTimer = GP::MenuInputDeadTime();

                m_text_Explanation2->flash(GP::MenuInputDeadTime());
            }

            if (JamTemplate::InputManager::justPressed(sf::Keyboard::Down)
                || JamTemplate::InputManager::justPressed(sf::Keyboard::S)) {
                m_currentLevel++;
                m_inputDeadTimer = GP::MenuInputDeadTime();
                float posOffset = 0;
                if (m_currentLevel >= GP::getLevelList().size()) {
                    m_currentLevel = 0;
                    posOffset = m_levelNames.size() * GP::GetMenuLevelTextDistance();
                }

                for (auto i = 0U; i != m_levelNames.size(); ++i) {
                    uint8_t a = static_cast<uint8_t>(JamTemplate::MathHelper::clamp(255
                            - static_cast<int>(
                                  std::fabs(static_cast<int>(i) - static_cast<int>(m_currentLevel)))
                                * 200,
                        0, 255));

                    auto twa = JamTemplate::TweenAlpha<SmartText>::create(
                        m_levelNames.at(i), 0.25f, m_levelNames.at(i)->getColor().a, a);
                    add(twa);

                    auto twp = JamTemplate::TweenPosition<SmartText>::create(m_levelNames.at(i),
                        GP::MenuInputDeadTime(), m_levelNames.at(i)->getPosition(),
                        m_levelNames.at(i)->getPosition()
                            - sf::Vector2f { 0, GP::GetMenuLevelTextDistance() - posOffset });
                    add(twp);

                    if (i == m_currentLevel) {
                        m_levelNames.at(i)->setShadow(
                            GP::PaletteFontShadow(), sf::Vector2f { 1, 1 });
                    } else {
                        m_levelNames.at(i)->setShadowActive(false);
                    }
                }
            }

            if (JamTemplate::InputManager::justPressed(sf::Keyboard::Up)
                || JamTemplate::InputManager::justPressed(sf::Keyboard::W)) {
                int cl = m_currentLevel;
                cl--;
                m_inputDeadTimer = GP::MenuInputDeadTime();

                float posOffset = 0;
                if (cl < 0) {
                    m_currentLevel = static_cast<int>(GP::getLevelList().size() - 1);
                    posOffset = m_levelNames.size() * GP::GetMenuLevelTextDistance();
                } else {
                    m_currentLevel = cl;
                }

                for (auto i = 0U; i != m_levelNames.size(); ++i) {
                    uint8_t a = static_cast<uint8_t>(JamTemplate::MathHelper::clamp(255
                            - static_cast<int>(
                                  std::fabs(static_cast<int>(i) - static_cast<int>(m_currentLevel)))
                                * 200,
                        0, 255));

                    auto twa = JamTemplate::TweenAlpha<SmartText>::create(
                        m_levelNames.at(i), 0.25f, m_levelNames.at(i)->getColor().a, a);
                    add(twa);

                    auto twp = JamTemplate::TweenPosition<SmartText>::create(m_levelNames.at(i),
                        GP::MenuInputDeadTime(), m_levelNames.at(i)->getPosition(),
                        m_levelNames.at(i)->getPosition()
                            + sf::Vector2f { 0, GP::GetMenuLevelTextDistance() - posOffset });
                    add(twp);

                    if (i == m_currentLevel) {
                        m_levelNames.at(i)->setShadow(
                            GP::PaletteFontShadow(), sf::Vector2f { 1, 1 });
                    } else {
                        m_levelNames.at(i)->setShadowActive(false);
                    }
                }
            }
        }

        for (auto const& st : m_levelNames) {
            st->update(elapsed);
        }
    }
}

void StateMenu::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());

    m_text_Title->draw(getGame()->getRenderTarget());

    for (auto const& st : m_levelNames) {
        st->draw(getGame()->getRenderTarget());
    }

    m_arrow->setPosition(sf::Vector2f { 20, 88 });
    m_arrow->setScale({ 1.0f, 1.0f });
    m_arrow->update(0.0f);
    m_arrow->draw(getGame()->getRenderTarget());

    m_arrow->setPosition(sf::Vector2f { 20, 128 });
    m_arrow->setScale({ 1.0f, -1.0f });
    m_arrow->update(0.0f);
    m_arrow->draw(getGame()->getRenderTarget());

    m_text_Explanation->draw(getGame()->getRenderTarget());
    m_text_Explanation2->draw(getGame()->getRenderTarget());

    m_text_Credits->draw(getGame()->getRenderTarget());

    m_overlay->draw(getGame()->getRenderTarget());
}
