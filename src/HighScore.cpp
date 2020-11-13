#include "HighScore.hpp"
#include <filesystem>

namespace {
constexpr auto filename = "data.tts";
constexpr std::size_t count = 5;

} // namespace

std::vector<float> loadHighscores()
{
    std::filesystem::path path { filename };
    if (!std::filesystem::exists(path)) {
        std::vector<float> v {};
        v.resize(count, 0.0f);
        return v;
    }

    std::ifstream in {};
    in.open(path, std::ios_base::binary);
    if (in.fail()) {
        throw std::logic_error { "cannot open highscores file for read." };
    }
    std::vector<float> scores;
    float a {};
    while (in >> a) {
        if (a < 0) {
            throw std::invalid_argument { "time score can not be negative." };
        }
        scores.push_back(a);
    }
    if (scores.size() != count) {
        throw std::logic_error { "highscore file does not contain correct amount of data." };
    }
    return scores;
}

void saveHighscores(std::vector<float> const& scores)
{
    if (scores.size() != count) {
        throw std::invalid_argument {
            "highscore list does not contain correct amount of entries."
        };
    }
    std::ofstream out {};
    out.open(filename, std::ios_base::binary);

    if (out.fail()) {
        throw std::logic_error { "cannot open highscores file for write." };
    }

    for (auto const& v : scores) {
        out << v << std::endl;
    }
}
