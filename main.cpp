#include <iostream>

#include "tools/option_parsing.hpp"
#include "render/rendering.hpp"

int main (int argc, char** argv)
{
    struct options opt = {};

    int test = parse_options(argc, argv, opt);

    if (test)
        return test;

    std::cout << "rx " << opt.rx << '\n'
              << "ry " << opt.ry << '\n'
              << "kb " << opt.kb << '\n'
              << "camx " << opt.camx << '\n'
              << "camy " << opt.camy << '\n'
              << "camz " << opt.camz << '\n'
              << "movespeed " << opt.movespeed << '\n'
              << "lookspeed " << opt.lookspeed << '\n'
              << "sx " << opt.sx << '\n'
              << "sy " << opt.sy << '\n'
              << "nx " << opt.nx << '\n'
              << "ny " << opt.ny << '\n'
              << "mf " << opt.mf << '\n'
              << "wspeed " << opt.wspeed << '\n'
              << "wforce " << opt.wforce << '\n'
              << "minw " << opt.minw << '\n'
              << "wheight " << opt.wheight << '\n'
              << "colr " << opt.colr << '\n'
              << "colg " << opt.colg << '\n'
              << "colb " << opt.colb << std::endl;

    return init(opt);
}