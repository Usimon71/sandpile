#include "../include/SandBox.h"
#include <limits>
#include <algorithm>

int32_t* FindCorners(const SandPile* SandPileArr, uint16_t n) {
    uint16_t min_x = std::numeric_limits<uint16_t>::max();
    uint16_t min_y = std::numeric_limits<uint16_t>::max();
    uint16_t max_x = 0;
    uint16_t max_y = 0;
    
    for (uint64_t i = 0; i != n; ++i) {
        
        min_x = std::min(min_x, SandPileArr[i].coord.x);
        max_x = std::max(max_x, SandPileArr[i].coord.x);
        min_y = std::min(min_y, SandPileArr[i].coord.y);
        max_y = std::max(max_y, SandPileArr[i].coord.y);

    }
    int32_t* res = new int32_t[4] {min_x, min_y, max_x, max_y};

    return res;
}



GridInit CreateGrid (const SandPile* SandPileArr, uint16_t n) {
    ToFallList tfl {new SandPile[n], n};
    uint64_t list_i = 0;
    int32_t* corners = FindCorners(SandPileArr, n);
    uint16_t side_x;
    uint16_t side_y;
    side_x = corners[2] - corners[0] + 1;
    side_y = corners[3] - corners[1] + 1;
    Coord bot_right {static_cast<uint16_t>(corners[2] - corners[0]), static_cast<uint16_t>(corners[3] - corners[1])};
    uint8_t** res_grid = new uint8_t*[side_y];
    for (uint16_t i = 0; i != side_y; ++i) {
        res_grid[i] = new uint8_t[side_x];
    }
    for (uint64_t i = 0; i != n; ++i) {
        uint16_t final_coord_x = SandPileArr[i].coord.x - corners[0];
        uint16_t final_coord_y = SandPileArr[i].coord.y - corners[1];

        if (SandPileArr[i].val < 4) {
            res_grid[final_coord_y][final_coord_x] = SandPileArr[i].val;
        } else {
            res_grid[final_coord_y][final_coord_x] = 9;
            tfl.list[list_i] = SandPile{Coord{final_coord_x, final_coord_y}, SandPileArr[i].val};
            ++list_i;
        }
    }
    return GridInit {Grid{res_grid, side_x, side_y, Coord{0, 0}, bot_right}, ToFallList{tfl.list, list_i}};
}

Grid GridExpand(const ToFallList& tfl, const Grid& grid_struct, bool side) {
    uint8_t** cur_grid = grid_struct.grid;
    uint16_t cur_side_x = grid_struct.side_x;
    uint16_t cur_side_y = grid_struct.side_y;
    SandPile* cur_tfl = tfl.list;
    uint64_t cur_tfl_n = tfl.n;
    if (side) { // expand vertically
        uint16_t new_side_y = cur_side_y * 2 + cur_side_y % 2;
        uint16_t offset_y = cur_side_y / 2 + cur_side_y % 2;
        Coord top_left {static_cast<uint16_t>(grid_struct.top_left.x), static_cast<uint16_t>(grid_struct.top_left.y + offset_y)};
        Coord bot_right {static_cast<uint16_t>(grid_struct.bot_right.x), static_cast<uint16_t>(grid_struct.bot_right.y + offset_y)};
        Grid new_grid {new uint8_t*[new_side_y], cur_side_x, new_side_y, top_left, bot_right};
        for (uint16_t i = 0; i != new_side_y; ++i) {
            new_grid.grid[i] = new uint8_t[cur_side_x] {0};
        }
        
        for (uint16_t i = 0; i != cur_side_y; ++i) {
            for (uint16_t j = 0; j != cur_side_x; ++j) {
                new_grid.grid[i + offset_y][j] = cur_grid[i][j];
            }
        }
        for (uint64_t i = 0; i != cur_tfl_n; ++i) {
            cur_tfl[i].coord.y += offset_y;
        }
        for (uint16_t i = 0; i != cur_side_y; ++i) {
            delete [] cur_grid[i];
        }
        delete [] cur_grid;

        return new_grid;
    } else { // expand horizontally
        uint16_t new_side_x = cur_side_x * 2 + cur_side_x % 2;
        uint16_t offset_x = cur_side_x / 2 + cur_side_x % 2;
        Coord top_left{static_cast<uint16_t>(grid_struct.top_left.x + offset_x), static_cast<uint16_t>(grid_struct.top_left.y)};
        Coord bot_right{static_cast<uint16_t>(grid_struct.bot_right.x + offset_x), static_cast<uint16_t>(grid_struct.bot_right.y)};
        Grid new_grid {new uint8_t*[cur_side_y], new_side_x, cur_side_y, top_left, bot_right};
        for (uint16_t i = 0; i != cur_side_y; ++i) {
            new_grid.grid[i] = new uint8_t[new_side_x] {0};
        }

        for (uint16_t i = 0; i != cur_side_y; ++i) {
            for (uint16_t j = 0; j != cur_side_x; ++j) {
                new_grid.grid[i][j + offset_x] = cur_grid[i][j];
            }
        }

        for (uint64_t i = 0; i != cur_tfl_n; ++i) {
            cur_tfl[i].coord.x += offset_x;
        }
        for (uint16_t i = 0; i != cur_side_y; ++i) {
            delete [] cur_grid[i];
        }
        delete [] cur_grid;
        return new_grid;
    }

}

ToFallList FallPiles(Grid& grid_struct, ToFallList& tfl) {
    Grid& grid_obj = grid_struct;
    SandPile*& to_fall_list = tfl.list;
    uint64_t& n = tfl.n;
    uint16_t& side_y = grid_obj.side_y;
    uint16_t& side_x = grid_obj.side_x;
    uint8_t**& grid = grid_obj.grid;
    uint16_t min_x = std::numeric_limits<uint16_t>::max();
    uint16_t min_y = std::numeric_limits<uint16_t>::max();
    uint16_t max_x = 0;
    uint16_t max_y = 0;
    for (uint64_t i = 0; i != n; ++i) {
        
        to_fall_list[i].val -= 4;
        if (to_fall_list[i].val >= 4) {
            grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x] = 9;
        } else {
            grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x] = to_fall_list[i].val;
        }
    }
    uint64_t cnt = n;
    for (int i = 0; i != n; ++i) {
        bool expand_y = false;
        bool expand_x = false;
        if (to_fall_list[i].coord.y - 1 < 0) {
            grid_obj = GridExpand(tfl, grid_struct, 1);
            expand_y = true;
        }
        ++grid[to_fall_list[i].coord.y - 1][to_fall_list[i].coord.x] >= 4 ? cnt += 1 : cnt;
        if (to_fall_list[i].coord.y - 1 < grid_obj.top_left.y) {
            grid_obj.top_left.y = to_fall_list[i].coord.y - 1;
        }
        
        if (!expand_y && (to_fall_list[i].coord.y + 1 > side_y - 1)) {
            grid_obj = GridExpand(tfl, grid_struct, 1);
        }
        ++grid[to_fall_list[i].coord.y + 1][to_fall_list[i].coord.x] >= 4 ? cnt += 1 : cnt;
        if (to_fall_list[i].coord.y + 1 > grid_obj.bot_right.y) {
            grid_obj.bot_right.y = to_fall_list[i].coord.y + 1;
       
        }
        if (to_fall_list[i].coord.x - 1 < 0) {
            grid_obj = GridExpand(tfl, grid_struct, 0);
            expand_x = true;
        }
        ++grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x - 1] >= 4 ? cnt += 1 : cnt;
        if (to_fall_list[i].coord.x - 1 < grid_obj.top_left.x) {
            grid_obj.top_left.x = to_fall_list[i].coord.x - 1;
        }

        if (!expand_x && (to_fall_list[i].coord.x + 1 > side_x - 1)) {
                grid_obj = GridExpand(tfl, grid_struct, 0);
        }
        ++grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x + 1] >= 4 ? cnt += 1 : cnt;
        if (to_fall_list[i].coord.x + 1 > grid_obj.bot_right.x) {
            grid_obj.bot_right.x = to_fall_list[i].coord.x + 1;
        }
    }
    
    SandPile* new_to_fall_list = new SandPile[cnt];
    uint16_t ntfl_i = 0;
    for (uint16_t i = 0; i != side_y; ++i) {
        for (uint16_t j = 0; j != side_x; ++j){
            if (grid[i][j] >= 4 && grid[i][j] <= 7){
                new_to_fall_list[ntfl_i] = SandPile{Coord{j, i}, grid[i][j]};
                ++ntfl_i;
            } else if (grid[i][j] >= 9 && grid[i][j] <= 13) {
                for (uint16_t k = 0; k != n; ++k) {
                    if (to_fall_list[k].coord.x == j && to_fall_list[k].coord.y == i) {
                        new_to_fall_list[ntfl_i] = SandPile{Coord{j, i}, grid[i][j] - 9 + to_fall_list[k].val};
                        ++ntfl_i;
                        break;
                    }
                }
            }
        }
    }
    delete [] to_fall_list;

    return ToFallList{new_to_fall_list, ntfl_i};

}