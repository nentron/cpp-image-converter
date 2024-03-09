#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <string_view>
#include <vector>

using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    BitmapFileHeader() = default;
    BitmapFileHeader(int stride, int h)
        : total_size(static_cast<uint32_t>(54 + stride * h))
    {
    }
    char tag[2] = {'B', 'M'};
    uint32_t total_size;
    uint32_t reserved_space = {};
    uint32_t pixel_offset = 54;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    BitmapInfoHeader() = default;

    BitmapInfoHeader(int w, int h, int stride)
        : width(static_cast<int32_t>(w))
        , height(static_cast<int32_t>(h))
        , image_size(static_cast<uint32_t>(static_cast<int32_t>(stride) * h))
    {}

    uint32_t size = 40;
    int32_t width;
    int32_t height;
    uint16_t spaces = 1;
    uint16_t bit_count = 24;
    uint32_t compress = 0;
    uint32_t image_size;
    int32_t x_pixels_per_meter = 11811;
    int32_t y_pixels_per_meter = 11811;
    int32_t used_colors = 0;
    int32_t sign_colors = 0x1000000;
}
PACKED_STRUCT_END

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image){
    std::ofstream ofs(file, std::ios::binary);

    BitmapFileHeader f_header{GetBMPStride(image.GetWidth()), image.GetHeight()};
    BitmapInfoHeader i_header{image.GetWidth(), image.GetHeight(), GetBMPStride(image.GetWidth())};

    ofs.write((char*)&f_header, 14);
    ofs.write((char*)&i_header, 40);

    vector<char> buff(GetBMPStride(image.GetWidth()), '\0');

    for (int y = image.GetHeight() - 1; y > -1; --y){
        const Color* line = image.GetLine(y);

        for (size_t x = 0; x < static_cast<size_t>(image.GetWidth()); ++x){
            buff[x * 3] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }

        ofs.write(buff.data(), buff.size());
    }

    return ofs.good();
}
// напишите эту функцию
Image LoadBMP(const Path& file){
    std::ifstream ifs(file, std::ios::binary);
    if (!ifs){
        return Image{};
    }

    BitmapFileHeader f_header;
    BitmapInfoHeader i_header;

    ifs.read(reinterpret_cast<char*>(&f_header), sizeof(f_header));
    ifs.read(reinterpret_cast<char*>(&i_header), sizeof(i_header));

    if (f_header.tag[0] != 'B' || f_header.tag[1] != 'M'){
        return Image{};
    }

    if (i_header.width <= 0 || i_header.height <= 0){
        return Image{};
    }

    Image result{
        static_cast<int>(i_header.width),
        static_cast<int>(i_header.height),
        Color::Black()
    };
    vector<char> buff(GetBMPStride(static_cast<int>(i_header.width)));
    for (int y = result.GetHeight() - 1; y > -1; --y){
        Color* line = result.GetLine(y);

        ifs.read(buff.data(), buff.size());

        for (size_t x = 0; x < static_cast<size_t>(result.GetWidth()); ++x){
            line[x].b = static_cast<byte>(buff[x * 3]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    return result;
}

}  // namespace img_lib