#pragma once

#include <cinttypes>
#include <filesystem>
#include <fstream>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t bfType {0x4D42};
    uint32_t bfSize;
    uint16_t bfReserved1 {0};
    uint16_t bfReserved2 {0};
    uint32_t bfOffBits ;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BMPInfo {
    uint32_t bcSize;
    uint32_t bcWidth;
    uint32_t bcHeight;
    uint16_t bcPlanes = 1;
    uint16_t bcBitCount = 4;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;
    int32_t biXPelsPerMete = 0;
    int32_t biYPelsPerMeter = 0;
    uint32_t biClrUsed = 4;
    uint32_t biClrImportant = 0;
};
#pragma pack(pop)

struct RGBQuad {
        uint8_t rgb_red;
        uint8_t rgb_green;
        uint8_t rgb_blue;
        uint8_t rgb_reserved = 0;
};

void CreateBmp(uint8_t** sandpile, std::filesystem::path file_output, uint16_t left_border, uint16_t right_border, 
                uint16_t top_border, uint16_t bottom_border);