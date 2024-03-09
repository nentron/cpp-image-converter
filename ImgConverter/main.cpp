#include <bmp_image.h>
#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

class ImageFormatInterface{
public:
    virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
    virtual img_lib::Image LoadImage(const img_lib::Path& file ) const = 0;
};


namespace FormatInterfaces{
    class JPEG : public ImageFormatInterface{
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SaveJPEG(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadJPEG(file);
        }
    };

    class PPM : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SavePPM(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadPPM(file);
        }
    };

    class BMP : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
            return img_lib::SaveBMP(file, image);
        }

        img_lib::Image LoadImage(const img_lib::Path& file) const override {
            return img_lib::LoadBMP(file);
        }
    };
}


enum class Format{
    UNKNOWN,
    JPEG,
    PPM,
    BMP
};

Format GetFormatByExtension(const img_lib::Path& input_file){
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    if (ext == ".bmp"sv){
        return Format::BMP;
    }

    return Format::UNKNOWN;
}

ImageFormatInterface* GetFormatInterface(const img_lib::Path& input_file){
    static const FormatInterfaces::JPEG jpegFormat;
    static const FormatInterfaces::PPM ppmFormat;
    static const FormatInterfaces::BMP bmpFormat;

    const ImageFormatInterface* jpeg_pointer = &jpegFormat;
    const ImageFormatInterface* ppm_pointer = &ppmFormat;
    const ImageFormatInterface* bmp_pointer = &bmpFormat;

    const Format format = GetFormatByExtension(input_file);
    switch (format){
        case Format::JPEG:
            return const_cast<ImageFormatInterface*>(jpeg_pointer);
        case Format::PPM:
            return const_cast<ImageFormatInterface*>(ppm_pointer);
        case Format::BMP:
            return const_cast<ImageFormatInterface*>(bmp_pointer);
        default:
            return nullptr;
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];

    const ImageFormatInterface* in_format = GetFormatInterface(in_path);
    if (!in_format){
        cerr << "Unknown format of the input file"sv << endl;
        return 2;
    }

    const ImageFormatInterface* out_format = GetFormatInterface(out_path);
    if (!out_format){
        cerr << "Unknown format of the output file"sv << endl;
        return 3;
    }


    img_lib::Image image = in_format -> LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    if (!out_format -> SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
}