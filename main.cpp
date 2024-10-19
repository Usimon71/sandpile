#include <iostream>
#include <limits>

#include "images.h"
#include "SandBox.h"
#include "bmp.h"
#include "parser.h"  

struct GridWthCorners {
    SandPile* arr;
    uint64_t n = 0;
    int32_t* corners;
};

GridWthCorners ParseTsv(const char* input) {
    std::ifstream input_file(input);
    uint64_t n = 0;
    uint16_t x;
    uint16_t y;
    uint64_t val;
    while (input_file >> x >> y >> val) {
        ++n;
    }
    
    SandPile* arr = new SandPile[n];

    input_file.clear();
    input_file.seekg(0, input_file.beg);

    uint16_t min_x = std::numeric_limits<uint16_t>::max();
    uint16_t min_y = std::numeric_limits<uint16_t>::max();
    uint16_t max_x = 0;
    uint16_t max_y = 0;

    uint64_t i = 0;
    while (input_file >> x >> y >> val) {
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);

        arr[i] = SandPile{{x, y}, val};
        ++i;
    }

    int32_t res[4] = {min_x, min_y, max_x, max_y};
    return GridWthCorners{arr, n, res};
}

int main(const int argc, const char** argv){
    const ParseResult pr = Parse(argc, argv);
    const char* input = pr.input;
    GridWthCorners gr_wth_corn = ParseTsv(pr.input);
    GridInit grid_init = CreateGrid(gr_wth_corn.arr, gr_wth_corn.n, gr_wth_corn.corners);
    CreateImagesFolder(pr, grid_init);
}

