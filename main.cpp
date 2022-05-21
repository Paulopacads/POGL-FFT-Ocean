#include <iostream>

#include "src/tools/option_parsing.hpp"

#define DEFAULT_RX 1920
#define DEFAULT_RY 1080

int main (int argc, char** argv)
{
    struct options opt = { DEFAULT_RX, DEFAULT_RY };

    int test = parse_options(argc, argv, opt);

    if (test)
        return test;

    std::cout << opt.rx << '\n'
              << opt.ry << std::endl;

    return test;
}