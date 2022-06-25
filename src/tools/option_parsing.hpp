#pragma once

struct options {
    int rx;
    int ry;
};

int parse_options(int argc, char** argv, struct options& opt);