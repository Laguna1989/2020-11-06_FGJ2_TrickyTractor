#ifndef GAME_HIGHSCORE_INCLUDEUARD_HPP
#define GAME_HIGHSCORE_INCLUDEUARD_HPP

#include <fstream>
#include <vector>

std::vector<float> loadHighscores();
void saveHighscores(std::vector<float> const& scores);

#endif
