#include "height_spectrum.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

Height::Height(const int xPoints, const int yPoints)
    : _xPoints(xPoints)
    , _yPoints(yPoints) {}

void Height::init_fonctor(const int i) {
    _currentX = i - _xPoints / 2;
    _currentY = -_yPoints / 2;
}

double Height::operator()() {
    ++_currentY;
    return sqrt(_philippsSpectrums[_currentX + _xPoints / 2][_currentY + _yPoints / 2] / 2) * gaussian();
}

void Height::generate_philipps(Philipps* const philipps) {
    _philippsSpectrums.resize(_xPoints + 1);
    for (int i = 0; i <= _xPoints; ++i) {
        _philippsSpectrums[i].resize(_yPoints + 1);
        philipps->init_fonctor(i);
        for (int j = 0; j <= _yPoints; ++j)
            _philippsSpectrums[i][j] = (*philipps)();
    }
}

double Height::gaussian() {
    double var1;
    double var2;
    double s;

    do {
        var1 = (rand() % 201 - 100) / 100.;
        var2 = (rand() % 201 - 100) / 100.;
        s = var1 * var1 + var2 * var2;
    } while (s >= 1 || s == 0);
    return var1 * sqrt(-log(s) / s);
}