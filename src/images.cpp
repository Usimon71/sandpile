#include <filesystem>
#include <fstream>
#include <cstring>
#include <cmath>
#include <limits>

#include "parser.h"
#include "SandBox.h"
#include "bmp.h"
#include "images.h"

const int kLenOfFile = 4;
const int kLenOfDotBmp = 4;

int LenInd(uint64_t ind) {
    return (ind == 0) ? 1 : trunc(log10(ind)) + 1;
}

char* IntToStr(uint64_t ind) {
    int len = LenInd(ind);
    char* res = new char[len + 1];
    res[len] = '\0';
    int i = len - 1;
    while (ind > 0) {
        res[i] =  static_cast<char>(ind % 10) + '0';
        ind /= 10;
        --i;
    }

    return res;
}

char* Path(const char* directory, const char* ind) {
    int len_dir = std::strlen(directory);
    int len_ind = std::strlen(ind);
    char* res = new char[len_dir + kLenOfFile + len_ind + kLenOfDotBmp];
    std::strcpy(res, directory);
    std::strcat(res, "file");
    std::strcat(res, ind);
    std::strcat(res, ".bmp");

    return res;
}

void CreateImage(const GridInit& grid_init, const char* directory, uint64_t i) {
    char* str_ind = IntToStr(i);
    int i_len = LenInd(i);
    const char* new_path = Path(directory, str_ind);
    std::filesystem::path path = new_path;
    CreateBmp(grid_init.grid.grid, path, grid_init.grid.top_left.x, grid_init.grid.bot_right.x, 
                        grid_init.grid.top_left.y, grid_init.grid.bot_right.y);
    delete str_ind;
    delete new_path;
}

void CreateImagesFolder(const ParseResult& pr, GridInit& grid_init) {
    ToFallList fl = grid_init.fall_list;  
    
    uint64_t max_iter = pr.max_iter;
    uint64_t freq = pr.freq;
    const char* directory = pr.output;
    std::filesystem::create_directories(directory);

    uint64_t i = 0;
    while (i < max_iter && !grid_init.grid.is_stable) {
        if (i % freq == 0) {
            CreateImage(grid_init, directory, i);
        }
        ++i;
        fl = FallPiles(grid_init.grid, fl); 
    }
    CreateImage(grid_init, directory, i);
}