#include <cmath>
#include <iostream>

#include "philipps_spectrum.hpp"

Philipps::Philipps(const double sceneWidth, const double sceneHeight,
    const int xPoints, const int yPoints,
    const double windSpeed, const int windForce,
    const double minWaveSize, const double waveHeight)
    : _sceneWidth(sceneWidth)
    , _sceneHeight(sceneHeight)
    , _xPoints(xPoints)
    , _yPoints(yPoints)
    , _windSpeed(windSpeed)
    , _windForce(windForce)
    , _minWaveSize(minWaveSize)
    , _waveHeight(waveHeight) {}

void Philipps::init_fonctor(const int i) {
    _currentX = i - _xPoints / 2;
    _currentY = -_yPoints / 2;
}

const double Philipps::operator()() {
    const double kx = 2 * M_PI * _currentX / _sceneWidth;
    const double ky = 2 * M_PI * _currentY / _sceneHeight;
    const double k2 = kx * kx + ky * ky;
    ++_currentY;

    if(!k2)
        return 0;
    const double l = _windSpeed * _windSpeed / 9.81;
    return _waveHeight * exp(-1 / (k2 * l * l) - k2 * _minWaveSize * _minWaveSize) * pow(kx * kx / k2, _windForce) / (k2 * k2);
}