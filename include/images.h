#pragma once

#include <filesystem>
#include <fstream>
#include <cstring>
#include <cmath>
#include <limits>

#include "parser.h"
#include "SandBox.h"
#include "bmp.h"
#include "images.h"

char* IntToStr(uint64_t ind, int len) {
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

char* Path(const char* directory, const char* ind, int len_ind, int len_dir) {
    char* res = new char[len_dir + 4 + len_ind + 4];
    std::strcpy(res, directory);
    std::strcat(res, "file");
    std::strcat(res, ind);
    std::strcat(res, ".bmp");
    return res;
}

void CreateImages(const ParseResult& pr, GridInit& grid_init) {
    
    ToFallList fl = grid_init.tfl;  
    
    uint64_t max_iter = pr.max_iter;
    uint64_t freq = pr.freq;
    const char* directory = pr.output;
    std::filesystem::create_directories(directory);
    int dir_len = std::strlen(directory);

    uint64_t i = 0;
    while (i < max_iter and !grid_init.grid.is_stable){
        if (i % freq == 0) {
            int i_len = (i == 0) ? 1 : trunc(log10(i)) + 1;
            
            char* str_ind = IntToStr(i, i_len);
            const char* new_path = Path(directory, str_ind, i_len, dir_len);
            std::filesystem::path path = new_path;
            CreateImage(grid_init.grid.grid, path, grid_init.grid.top_left.x, grid_init.grid.bot_right.x, 
                        grid_init.grid.top_left.y, grid_init.grid.bot_right.y);
        }
        ++i;
        fl = FallPiles(grid_init.grid, fl);
    }
    int i_len = (i == 0) ? 1 : trunc(log10(i)) + 1;
    char* str_ind = IntToStr(i, i_len);
    const char* new_path = Path(directory, str_ind, i_len, dir_len);
    std::filesystem::path path = new_path;
    CreateImage(grid_init.grid.grid, path, grid_init.grid.top_left.x, grid_init.grid.bot_right.x, 
                grid_init.grid.top_left.y, grid_init.grid.bot_right.y);
}