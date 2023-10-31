#include "../include/SandBox.h"
#include <limits>
#include <cmath>
#include <algorithm>

int32_t* FindCorners(const SandPile* SandPileArr, uint16_t n) {
    uint16_t min_x = std::numeric_limits<uint16_t>::max();
    uint16_t min_y = std::numeric_limits<uint16_t>::max();
    uint16_t max_x = 0;
    uint16_t max_y = 0;
    
    int32_t grid_x1 = std::numeric_limits<int32_t>::max();
    int32_t grid_y1 = std::numeric_limits<int32_t>::max();;
    int32_t grid_x2 = 0;
    int32_t grid_y2 = 0;
    
    for (uint64_t i = 0; i != n; ++i) {
        uint16_t cells_expand = (std::log(SandPileArr[i].val) / std::log(4)) + 1;
        
        min_x = std::min(min_x, SandPileArr[i].coord.x);
        max_x = std::max(max_x, SandPileArr[i].coord.x);
        min_y = std::min(min_y, SandPileArr[i].coord.y);
        max_y = std::max(max_y, SandPileArr[i].coord.y);

        grid_x2 = std::max(grid_x2, static_cast<int32_t>(SandPileArr[i].coord.x + cells_expand));
        grid_x1 = std::min(grid_x1, static_cast<int32_t>(SandPileArr[i].coord.x - cells_expand));
        grid_y2 = std::max(grid_y2, static_cast<int32_t>(SandPileArr[i].coord.y + cells_expand));
        grid_y1 = std::min(grid_y1, static_cast<int32_t>(SandPileArr[i].coord.y - cells_expand));
    }
    int32_t* res = new int32_t[8] {min_x, min_y, max_x, max_y, grid_x1, grid_x2, grid_y1,  grid_y2};

    return res;
}



GridInit CreateGrid (const SandPile* SandPileArr, uint16_t n) {
    ToFallList tfl = {new SandPile[n], n};
    uint64_t list_i = 0;
    int32_t* corners = FindCorners(SandPileArr, n);
    uint16_t side_x;
    uint16_t side_y;
    side_x = corners[5] - corners[4] + 1;
    side_y = corners[7] - corners[6] + 1;
    uint8_t** res_grid = new uint8_t*[side_y];
    for (uint16_t i = 0; i != side_y; ++i) {
        res_grid[i] = new uint8_t[side_x];
    }
    for (uint64_t i = 0; i != n; ++i) {
        uint16_t final_coord_x = SandPileArr[i].coord.x - corners[4];
        uint16_t final_coord_y = SandPileArr[i].coord.y - corners[6];

        if (SandPileArr[i].val < 4) {
            res_grid[final_coord_y][final_coord_x] = SandPileArr[i].val;
        } else {
            res_grid[final_coord_y][final_coord_x] = 9;
            tfl.list[list_i] = SandPile{Coord{final_coord_x, final_coord_y}, SandPileArr[i].val};
            ++list_i;
        }
    }

    return GridInit {Grid{res_grid, side_x, side_y}, ToFallList{tfl.list, list_i}};
}

ToFallList FallPiles(const Grid& grid_struct, const ToFallList& tfl) {
    SandPile* to_fall_list = tfl.list;
    uint16_t n = tfl.n;
    
    uint16_t side_x = grid_struct.side_x;
    uint16_t side_y = grid_struct.side_y;
    uint8_t** grid = grid_struct.grid;
    for (int i = 0; i != n; ++i) {
        
        to_fall_list[i].val -= 4;
        if (to_fall_list[i].val >= 4) {
            grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x] = 9;
        } else {
            grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x] = to_fall_list[i].val;
        }
    }
    uint64_t cnt = n;
    for (int i = 0; i != n; ++i) {
        ++grid[to_fall_list[i].coord.y - 1][to_fall_list[i].coord.x] >= 4 ? cnt += 1 : cnt;
        ++grid[to_fall_list[i].coord.y + 1][to_fall_list[i].coord.x] >= 4 ? cnt += 1 : cnt;
        ++grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x + 1] >= 4 ? cnt += 1 : cnt;
        ++grid[to_fall_list[i].coord.y][to_fall_list[i].coord.x - 1] >= 4 ? cnt += 1 : cnt;
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