#pragma once

#include <iostream>
#include <cinttypes>

struct Coord {
    uint16_t x = 0;
    uint16_t y = 0;
};

struct SandPile {
    Coord coord {0, 0};
    uint64_t val = 0;
};

struct ToFallList {
    SandPile* list;
    uint64_t n;
};

struct Grid {
    Grid(uint8_t** grid, uint16_t side_x, uint16_t side_y, Coord top_left, Coord bottom_right) {
        grid = grid;
        side_x = side_x;
        side_y = side_y;
        top_left = top_left;
        bottom_right = bottom_right;
    };
    uint8_t** grid;
    uint16_t side_x;
    uint16_t side_y;
    Coord top_left {0, 0};
    Coord bot_right {0, 0};
    bool is_stable = false;
};

struct GridInit {
    Grid grid;
    ToFallList fall_list;
};



GridInit CreateGrid (const SandPile* SandPileArr, uint16_t n, const int32_t* corners);

ToFallList FallPiles (Grid& grid_struct, ToFallList& tfl);