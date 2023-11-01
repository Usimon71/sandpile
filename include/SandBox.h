#include <iostream>
#include <cinttypes>

#pragma once

struct Coord {
    uint16_t x = 0;
    uint16_t y = 0;
};

struct SandPile {
    Coord coord {0, 0};
    uint64_t val = 0;
};

struct SandBox {
    Coord left_up {0, 0};
    Coord right_down {0, 0};
};

struct ToFallList {
    SandPile* list;
    uint64_t n;
};

struct Grid {
    uint8_t** grid;
    uint16_t side_x;
    uint16_t side_y;
    Coord top_left {0, 0};
    Coord bot_right {0, 0};
    bool is_stable = false;
};

struct GridInit {
    Grid grid;
    ToFallList tfl;
};



GridInit CreateGrid (const SandPile* SandPileArr, uint16_t n);

ToFallList FallPiles(Grid& grid_struct, ToFallList& tfl);