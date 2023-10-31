#include <iostream>
#include <filesystem>
#include <fstream>

#include "include/SandBox.h"
#include "include/bmp.h"

int main(){
    uint64_t n = 1;
    
    SandPile* arr = new SandPile[n];
    
    arr[0] = SandPile{{0, 0}, 44};
    //arr[1] = SandPile{{1, 1}, 5};
    // arr[1] = SandPile{{0, 2}, 9};
    std::filesystem::path path = "/run/media/don_simon/да это жёстко/ITMO_IS_1semester/cpp/proga/labwork3-Usimon71/images/file1.bmp";
    
    GridInit grid_init = CreateGrid(arr, n);
    uint64_t iter = 55;
    // std::cout << grid_init.grid.top_left.x << " " << grid_init.grid.top_left.y << "     " <<
    //                  grid_init.grid.bot_right.x << " " << grid_init.grid.bot_right.y << std::endl;
    ToFallList fl = grid_init.tfl;
    for (uint64_t i = 0; i != iter; ++i) {
        fl = FallPiles(grid_init.grid, fl);
        // std::cout << grid_init.grid.top_left.x << " " << grid_init.grid.bot_right.x << "     " <<
        //             grid_init.grid.top_left.y << " " << grid_init.grid.bot_right.y << std::endl;
           
    }
    for (uint16_t i = grid_init.grid.top_left.y; i != grid_init.grid.bot_right.y + 1; ++i) {
        for (uint16_t j = grid_init.grid.top_left.x; j != grid_init.grid.bot_right.x + 1; ++j) {
            std::cout << static_cast<int>(grid_init.grid.grid[i][j]) << " ";
        }
        std::cout << '\n';
    }
    CreateImage(grid_init.grid.grid, path, grid_init.grid.top_left.x, grid_init.grid.bot_right.x, 
                    grid_init.grid.top_left.y, grid_init.grid.bot_right.y);
}
