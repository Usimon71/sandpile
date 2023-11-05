#include <cstring>
#include <iostream>

#include "../include/parser.h"


uint64_t StrToInt(const char* str, int str_len) {
    uint64_t res = 0;
    for (int i = 0; i != str_len; ++i) {
        res *= 10;
        res += static_cast<uint64_t>(str[i] - '0');
    }
    return res;
}

ParseResult Parse(const int argc, const char** argv) {
    ParseResult pr;
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "-i") || !std::strcmp(argv[i], "--input")) {
            if (i + 1 >= argc) {
                std::cerr << "Please, write a correct command";
                break;
            }
            if (std::strlen(argv[i + 1]) <= 4) {
                std::cerr << "Please, write a correct command";
                break;
            }
            pr.input = argv[i + 1];
        } else if (!std::strcmp(argv[i], "-o") || !std::strcmp(argv[i], "--output")) {  
            if (i + 1 >= argc) {
                std::cerr << "Please, write a correct command";
                break;
            }
            pr.output = argv[i + 1];
        } else if (!std::strcmp(argv[i], "-m") || !std::strcmp(argv[i], "--max-iter")) {
            if (i + 1 >= argc) {
                std::cerr << "Please, write a correct command";
                break;
            }
            pr.max_iter = StrToInt(argv[i + 1], std::strlen(argv[i + 1]));
        } else if (!std::strcmp(argv[i], "-f") || !std::strcmp(argv[i], "--freq")) {
            if (i + 1 >= argc) {
                std::cerr << "Please, write a correct command";
                break;
            }
            pr.freq = StrToInt(argv[i + 1], std::strlen(argv[i + 1]));
        }
    }
    
    return pr;
}
