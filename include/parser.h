#pragma once

#include <cinttypes>

struct ParseResult {
    uint64_t max_iter = 10000000;
    uint64_t freq = 1;
    const char* output = "images/";
    const char* input = "input.tsv";
};

ParseResult Parse(const int argc, const char** argv);