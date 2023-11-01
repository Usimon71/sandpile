#include <iostream>
#include <filesystem>
#include <fstream>
//#include <string>

#include "../include/bmp.h"
//#include <windows.h>

RGBQuad palette[5] = {
    RGBQuad {255, 255, 255, 0}, //white
    RGBQuad{0, 255, 0, 0}, //green
    RGBQuad{0, 255, 255, 0}, //yellow
    RGBQuad{130, 0, 70, 0}, //violet
    RGBQuad{0, 0, 0, 0} //black
    
};

void CreateImage(uint8_t** sandpile, std::filesystem::path file_output, uint16_t left_border, uint16_t right_border, uint16_t top_border, uint16_t bottom_border) {
    
    uint16_t height = bottom_border - top_border + 1;
    uint16_t width = right_border - left_border + 1;
    std::ofstream BMP_file_out(file_output, std::ios::binary);
    std::cout << height << " " << width << std::endl;;
    BMPFileHeader file_header;
    std::cout << sizeof(BMPFileHeader) + sizeof(BMPInfo) << "       \n";
    file_header.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfo) + (height*width) / 2;
    file_header.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfo) + sizeof(palette);

    BMPInfo info_header;
    info_header.bcSize = sizeof(BMPInfo);
    info_header.bcWidth = width;
    info_header.bcHeight = -height;
    info_header.bcBitCount = 4;
    info_header.biClrUsed = 4;
    info_header.biClrImportant = 0;

    BMP_file_out.write(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader));
    BMP_file_out.write(reinterpret_cast<char*>(&info_header), sizeof(BMPInfo));
    BMP_file_out.write(reinterpret_cast<char*>(&palette), sizeof(palette));

    int bonus = (4 - ((width + 1) / 2) % 4) % 4;
    std::cout << bonus << '\n';
    for (int i = top_border; i <= bottom_border; ++i) {
        for (int j = left_border; j <= right_border; j += 2) {
            
            uint8_t res = ((sandpile[i][j] << 4) | sandpile[i][j + 1]);
           
            BMP_file_out.put(res);
        }
        for (int k = 0; k < bonus; ++k) {
            BMP_file_out.put(0);
        }
    }
    BMP_file_out.close();
}