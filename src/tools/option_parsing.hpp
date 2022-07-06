#pragma once

#include "camera/camera.hpp"

struct options {
    int rx = 1920;
    int ry = 1080;
    Camera::KEYBOARD kb = Camera::QWERTY;
    int camx = 20;
    int camy = 20;
    int camz = 20;
    float movespeed = .1;
    float lookspeed = .001;
    int sx = 350;
    int sy = 350;
    int nx = 128;
    int ny = 128;
    float mf = .8;
    float wspeed = 50;
    float wforce = 2;
    float minw = .1;
    float wheight = .000005;
    float colr = .212;
    float colg = .611;
    float colb = 1.;
};

int parse_options(int argc, char** argv, struct options& opt);