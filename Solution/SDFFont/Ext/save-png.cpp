
#include "save-png.h"

#include "../core/arithmetics.hpp"
//#include <lodepng.h>
#include<vector>
#include<stb\stb_image_write.h>
namespace msdfgen {

bool savePng(const Bitmap<float> &bitmap, const char *filename) {
    std::vector<unsigned char> pixels(bitmap.width()*bitmap.height());
    std::vector<unsigned char>::iterator it = pixels.begin();
    for (int y = bitmap.height()-1; y >= 0; --y)
        for (int x = 0; x < bitmap.width(); ++x)
            *it++ = clamp(int(bitmap(x, y)*0x100), 0xff);
    return stbi_write_png(filename, bitmap.width(), bitmap.height(), 1, &pixels[0], bitmap.width());
}

bool savePng(const Bitmap<FloatRGB> &bitmap, const char *filename) {
    std::vector<unsigned char> pixels(3*bitmap.width()*bitmap.height());
    std::vector<unsigned char>::iterator it = pixels.begin();
    for (int y = bitmap.height()-1; y >= 0; --y)
        for (int x = 0; x < bitmap.width(); ++x) {
            *it++ = clamp(int(bitmap(x, y).r*0x100), 0xff);
            *it++ = clamp(int(bitmap(x, y).g*0x100), 0xff);
            *it++ = clamp(int(bitmap(x, y).b*0x100), 0xff);
        }
    return stbi_write_png(filename, bitmap.width(), bitmap.height(), 3, &pixels[0], bitmap.width());
}

}
