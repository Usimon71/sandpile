#include <iostream>
#include "include/SandBox.h"

int main(){
    uint64_t n = 2;
    
    SandPile* arr = new SandPile[n];
    
    arr[0] = SandPile{Coord{2, 2}, 100};
    
    
    GridInit grid_init = CreateGrid(arr, n);
    
    uint64_t iter = 60;
    
    ToFallList fl = grid_init.tfl;
    for (int i = 0; i != iter; ++i) {
        fl = FallPiles(grid_init.grid, fl);
        for (uint16_t i = 0; i != grid_init.grid.side_y; ++i) {
            for (int j = 0; j != grid_init.grid.side_x; ++j) {
                std::cout << static_cast<int>(grid_init.grid.grid[i][j]) << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
