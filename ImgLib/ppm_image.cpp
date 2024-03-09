#include "ppm_image.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

static const string_view PPM_SIG = "P6"sv;
static const int PPM_MAX = 255;

// реализуйте эту функцию самостоятельно
bool SavePPM(const Path& file, const Image& image){
    ofstream ofs(file, ios::binary);

    ofs << PPM_SIG << '\n'
        << image.GetWidth() << ' ' << image.GetHeight() << '\n' 
        << PPM_MAX << '\n';

    std::vector<char> buff(static_cast<size_t>(image.GetWidth() * 3));

    for (int y = 0; y < image.GetHeight(); ++y){
        const Color* line = image.GetLine(y);
        for (size_t x = 0; x < static_cast<size_t>(image.GetWidth()); ++x){
            buff[x * 3 + 0] = static_cast<char>(line[x].r);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].b);
        }

        ofs.write(buff.data(), image.GetWidth() * 3);
    }

    return ofs.good();
}

Image LoadPPM(const Path& file) {

    ifstream ifs(file, ios::binary);
    std::string sign;
    int w, h, color_max;

    ifs >> sign >> w >> h >> color_max;

    if (sign != PPM_SIG || color_max != PPM_MAX) {
        return {};
    }

    const char next = static_cast<char>(ifs.get());
    if (next != '\n') {
        return {};
    }

    Image result(w, h, Color::Black());
    std::vector<char> buff(static_cast<size_t>(w * 3));

    for (int y = 0; y < h; ++y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), w * 3);

        for (size_t x = 0; x < static_cast<size_t>(w); ++x) {
            line[x].r = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].b = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    return result;
}

}  // namespace img_lib