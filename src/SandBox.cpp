#include <limits>
#include <algorithm>
#include <cstring>

#include "SandBox.h"

const uint8_t kBlackDetect = 4;
const uint8_t kWasInTfl = 9;
const uint8_t kMaxIfWasntInTfl = 7;
const uint8_t kMaxIfWasInTfl = 13;

bool ToExpandY(const Grid& grid, const ToFallList& to_fall_list) {
    bool expand_y = false;
    for (uint64_t i = 0; i != to_fall_list.n; ++i) {
        if (to_fall_list.list[i].coord.y - 1 < 0 || to_fall_list.list[i].coord.y + 1 > grid.side_y - 1) {
            expand_y = true;
        }
    }
    
    return expand_y;
}

bool ToExpandX(const Grid& grid, const ToFallList& to_fall_list) {
    bool expand_x = false;
    for (uint64_t i = 0; i != to_fall_list.n; ++i) {
        if (to_fall_list.list[i].coord.x - 1 < 0 || to_fall_list.list[i].coord.x + 1 > grid.side_x - 1) {
            expand_x = true;
        }
    }

    return expand_x;
}

GridInit CreateGrid(const SandPile* SandPileArr, uint16_t n, const int32_t* corners) {
    ToFallList to_fall_list {new SandPile[n], n};
    uint64_t list_i = 0;
    uint16_t side_x;
    uint16_t side_y;
    side_x = corners[2] - corners[0] + 1;
    side_y = corners[3] - corners[1] + 1;
    Coord bot_right {static_cast<uint16_t>(corners[2] - corners[0]), static_cast<uint16_t>(corners[3] - corners[1])};

    uint8_t** res_grid = new uint8_t*[side_y];
    for (uint16_t i = 0; i != side_y; ++i) {
        res_grid[i] = new uint8_t[side_x] {0};
    }

    for (uint64_t i = 0; i != n; ++i) {
        uint16_t final_coord_x = SandPileArr[i].coord.x - corners[0];
        uint16_t final_coord_y = SandPileArr[i].coord.y - corners[1];

        if (SandPileArr[i].val < kBlackDetect) {
            res_grid[final_coord_y][final_coord_x] = SandPileArr[i].val;
        } else {
            res_grid[final_coord_y][final_coord_x] = kBlackDetect;
            to_fall_list.list[list_i] = SandPile{{final_coord_x, final_coord_y}, SandPileArr[i].val};
            ++list_i;
        }
    }

    delete [] SandPileArr;

    auto g = Grid(res_grid, side_x, side_y, {0, 0}, bot_right);
    std::cout << "here\n";
    return GridInit {g, {to_fall_list.list, list_i}};
}

Grid GridExpand(const ToFallList& to_fall_list, const Grid& grid, bool side) {
    if (side) { // expand vertically
        uint16_t new_side_y = grid.side_y * 2 + grid.side_y % 2;
        uint16_t offset_y = grid.side_y / 2 + grid.side_y % 2;
        Coord top_left {static_cast<uint16_t>(grid.top_left.x),
                        static_cast<uint16_t>(grid.top_left.y + offset_y)};
        Coord bot_right {static_cast<uint16_t>(grid.bot_right.x), 
                         static_cast<uint16_t>(grid.bot_right.y + offset_y)};
        Grid new_grid {new uint8_t*[new_side_y], grid.side_x, new_side_y, top_left, bot_right};

        for (uint16_t i = 0; i != new_side_y; ++i) {
            new_grid.grid[i] = new uint8_t[grid.side_x] {0};
        }
        
        for (uint16_t i = 0; i != grid.side_y; ++i) {
            std::memcpy(new_grid.grid[i + offset_y], grid.grid[i], grid.side_x);
        }
        for (uint64_t i = 0; i != to_fall_list.n; ++i) {
            to_fall_list.list[i].coord.y += offset_y;
        }
        
        for (uint16_t i = 0; i != grid.side_y; ++i) {
            delete [] grid.grid[i];
        }
        delete [] grid.grid;

        return new_grid;
    } else { // expand horizontally
        uint16_t new_side_x = grid.side_x * 2 + grid.side_x % 2;
        uint16_t offset_x = grid.side_x / 2 + grid.side_x % 2;
        Coord top_left{static_cast<uint16_t>(grid.top_left.x + offset_x),
                       static_cast<uint16_t>(grid.top_left.y)};
        Coord bot_right{static_cast<uint16_t>(grid.bot_right.x + offset_x), 
                        static_cast<uint16_t>(grid.bot_right.y)};
        Grid new_grid {new uint8_t*[grid.side_y], new_side_x, grid.side_y, top_left, bot_right};
        for (uint16_t i = 0; i != grid.side_y; ++i) {
            new_grid.grid[i] = new uint8_t[new_side_x] {0};
        }

        for (uint16_t i = 0; i != grid.side_y; ++i) {
            std::memcpy(new_grid.grid[i] + offset_x, grid.grid[i], grid.side_x);
        }

        for (uint64_t i = 0; i != to_fall_list.n; ++i) {
            to_fall_list.list[i].coord.x += offset_x;
        }
        for (uint16_t i = 0; i != grid.side_y; ++i) {
            delete [] grid.grid[i];
        }
        delete [] grid.grid;

        return new_grid;
    }
}

ToFallList FallPiles(Grid& grid, ToFallList& to_fall_list) {
    uint16_t min_x = std::numeric_limits<uint16_t>::max();
    uint16_t min_y = std::numeric_limits<uint16_t>::max();
    uint16_t max_x = 0;
    uint16_t max_y = 0;
    
    if (to_fall_list.n == 0) {
        grid.is_stable = true;
        return to_fall_list;
    }
    
    for (uint64_t i = 0; i != to_fall_list.n; ++i) {
        to_fall_list.list[i].val -= 4;
        if (to_fall_list.list[i].val >= kBlackDetect) {
            grid.grid[to_fall_list.list[i].coord.y][to_fall_list.list[i].coord.x] = kWasInTfl;
        } else {
            grid.grid[to_fall_list.list[i].coord.y][to_fall_list.list[i].coord.x] = to_fall_list.list[i].val;
        }
    }

    if (ToExpandY(grid, to_fall_list)) {
        grid = GridExpand(to_fall_list, grid, true);
    }
    if (ToExpandX(grid, to_fall_list)) {
        grid = GridExpand(to_fall_list, grid, false);
    }

    uint64_t cnt = to_fall_list.n;
    for (uint64_t i = 0; i != to_fall_list.n; ++i) {
        ++grid.grid[to_fall_list.list[i].coord.y - 1][to_fall_list.list[i].coord.x] >= kBlackDetect ? cnt += 1 : cnt;
        if (to_fall_list.list[i].coord.y - 1 < grid.top_left.y) {
            grid.top_left.y = to_fall_list.list[i].coord.y - 1;
        }

        ++grid.grid[to_fall_list.list[i].coord.y + 1][to_fall_list.list[i].coord.x] >= kBlackDetect ? cnt += 1 : cnt;
        if (to_fall_list.list[i].coord.y + 1 > grid.bot_right.y) {
            grid.bot_right.y = to_fall_list.list[i].coord.y + 1;
       
        }

        ++grid.grid[to_fall_list.list[i].coord.y][to_fall_list.list[i].coord.x - 1] >= kBlackDetect ? cnt += 1 : cnt;
        if (to_fall_list.list[i].coord.x - 1 < grid.top_left.x) {
            grid.top_left.x = to_fall_list.list[i].coord.x - 1;
        }

        ++grid.grid[to_fall_list.list[i].coord.y][to_fall_list.list[i].coord.x + 1] >= kBlackDetect ? cnt += 1 : cnt;
        if (to_fall_list.list[i].coord.x + 1 > grid.bot_right.x) {
            grid.bot_right.x = to_fall_list.list[i].coord.x + 1;
        }
    }
    
    SandPile* new_to_fall_list = new SandPile[cnt];
    uint16_t ntfl_i = 0;
    for (uint16_t i = 0; i != grid.side_y; ++i) {
        for (uint16_t j = 0; j != grid.side_x; ++j) {
            if (grid.grid[i][j] >= kBlackDetect && grid.grid[i][j] <= kMaxIfWasntInTfl){ 
                new_to_fall_list[ntfl_i] = SandPile{Coord{j, i}, grid.grid[i][j]};
                grid.grid[i][j] = kBlackDetect;
                ++ntfl_i;
            } else if (grid.grid[i][j] >= kWasInTfl && grid.grid[i][j] <= kMaxIfWasInTfl) {
                for (uint16_t k = 0; k != to_fall_list.n; ++k) {
                    if (to_fall_list.list[k].coord.x == j && to_fall_list.list[k].coord.y == i) {
                        new_to_fall_list[ntfl_i] = SandPile{Coord{j, i}, grid.grid[i][j] - kWasInTfl + to_fall_list.list[k].val};
                        ++ntfl_i;
                        grid.grid[i][j] = kBlackDetect;
                        break;
                    }
                }
            }
        }
    }
    delete [] to_fall_list.list;

    return ToFallList{new_to_fall_list, ntfl_i};
}