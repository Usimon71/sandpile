#include <iostream>
#include <filesystem>
#include <fstream>

#include "include/SandBox.h"
#include "include/bmp.h"

int main(){
    uint64_t n = 1;
    
    SandPile* arr = new SandPile[n];
    
    arr[0] = SandPile{{0, 0}, 10000};
    std::filesystem::path path = "images/file1.bmp";
    
    GridInit grid_init = CreateGrid(arr, n);
    uint64_t max_iter = 10000;
    
    ToFallList fl = grid_init.tfl;
    uint64_t i = 0;

    while (i < max_iter and !grid_init.grid.is_stable){
        fl = FallPiles(grid_init.grid, fl);
        ++i;
    }
    // for (uint64_t i = 0; i != max_iter; ++i) {
    //     fl = FallPiles(grid_init.grid, fl);
           
    // }
    // for (uint16_t i = grid_init.grid.top_left.y; i != grid_init.grid.bot_right.y + 1; ++i) {
    //     for (uint16_t j = grid_init.grid.top_left.x; j != grid_init.grid.bot_right.x + 1; ++j) {
    //         std::cout << static_cast<int>(grid_init.grid.grid[i][j]) << " ";
    //     }
    //     std::cout << '\n';
    // }
    CreateImage(grid_init.grid.grid, path, grid_init.grid.top_left.x, grid_init.grid.bot_right.x, 
                    grid_init.grid.top_left.y, grid_init.grid.bot_right.y);
}
