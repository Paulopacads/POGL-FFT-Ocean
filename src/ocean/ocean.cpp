#include "ocean.hpp"

#include <GL/freeglut.h>
#include <iostream>
    
const int Ocean::get_scene_width() {
    return _sceneWidth;
}

const int Ocean::get_scene_height() {
    return _sceneHeight;
}

const int Ocean::get_x_points() {
    return _xPoints;
}

const int Ocean::get_y_points() {
    return _yPoints;
}

Ocean::Ocean(const double sceneWidth, const double sceneHeight, const int xPoints, const int yPoints, const double motionFactor)
    : _sceneWidth(sceneWidth)
    , _sceneHeight(sceneHeight)
    , _xPoints(xPoints)
    , _yPoints(yPoints)
    , _motionFactor(motionFactor) {

    _realSpectrum.resize(_xPoints + 1);
    _imagSpectrum.resize(_xPoints + 1);

    _realFrequency.resize(_xPoints + 1);
    _imagFrequency.resize(_xPoints + 1);

    std::vector<std::vector<double>>::iterator currReal;
    std::vector<std::vector<double>>::iterator currImag;
    for (currReal = _realFrequency.begin(), currImag = _imagFrequency.begin();
        currReal != _realFrequency.end() && currImag != _imagFrequency.end();) {
        currReal++->resize(_yPoints + 1);
        currImag++->resize(_yPoints + 1);
    }

    _realTime.resize(_yPoints + 1);
    _imagTime.resize(_yPoints + 1);

    for (currReal = _realTime.begin(), currImag = _imagTime.begin();
        currReal != _realTime.end() && currImag != _imagTime.end();) {
        currReal++->resize(_xPoints + 1);
        currImag++->resize(_xPoints + 1);
    }

    _yFFT.reserve(_xPoints);
    _xFFT.reserve(_yPoints);

    for(int i = 0; i < _xPoints; ++i)
        _yFFT.push_back(new FFT(_yPoints, &_realFrequency[i], &_imagFrequency[i]));

    for(int i = 0; i < _yPoints; ++i)
        _xFFT.push_back(new FFT(_xPoints, &_realTime[i], &_imagTime[i]));
}

Ocean::~Ocean() {
    for(int i = 0; i < _xPoints; ++i)
        delete _yFFT[i];

    for(int i = 0; i < _yPoints; ++i)
        delete _xFFT[i];
}

void Ocean::generate_height(Height* const height) {
    for (int i = 0; i <= _xPoints; ++i) {
        _realSpectrum[i].resize(_yPoints + 1);
        height->init_fonctor(i);
        for (int j = 0; j <= _yPoints; ++j)
            _realSpectrum[i][j] = (*height)();
    }

    for (int i = 0; i <= _xPoints; ++i) {
        _imagSpectrum[i].resize(_yPoints + 1);
        height->init_fonctor(i);
        for (int j = 0; j <= _yPoints; ++j)
            _imagSpectrum[i][j] = (*height)();
    }
}

void Ocean::operator()() {
    for(int i = 0; i < _xPoints; ++i) {
        get_sine_amp(i, _motionFactor * glutGet(GLUT_ELAPSED_TIME) / 1000., &_realFrequency[i], &_imagFrequency[i]);
        _yFFT[i]->compute(true);
    }

    for(int i = 0; i < _yPoints; ++i) {
        int j = 0;
        std::vector<double>::iterator currReal, currImag;
        for (currReal = _realTime[i].begin(), currImag = _imagTime[i].begin();
            currReal != _realTime[i].end() && currImag != _imagTime[i].end(); ++j) {
            *currReal++ = _realFrequency[j][i];
            *currImag++ = _imagFrequency[j][i];
        }

        _xFFT[i]->compute(true);
    }
}

void Ocean::get_sine_amp(const int x, const double time, std::vector<double>* const realFrequencySlice, std::vector<double>* const imagFrequencySlice) const {
    float pix = 2 * M_PI * x / _sceneWidth;
    int i = 0;
    std::vector<double>::iterator currReal, currImag;
    for (currReal = realFrequencySlice->begin(), currImag = imagFrequencySlice->begin();
        currReal != realFrequencySlice->end() && currImag != imagFrequencySlice->end(); ++i) {
        float piy = 2 * M_PI * i / _sceneHeight;
        float pixy = pix * pix + piy * piy;
        float A = time * sqrt(9.81 * sqrt(pixy) * (1 + pixy / 100));

        *currReal++ = _realSpectrum[x][i] * cos(A)
            - _imagSpectrum[x][i] * sin(A)
            + _realSpectrum[_xPoints - x][_yPoints - i] * cos(-A)
            + _imagSpectrum[_xPoints - x][_yPoints - i] * sin(-A);
        *currImag++ = _imagSpectrum[x][i] * cos(A)
            + _realSpectrum[x][i] * sin(A)
            - _imagSpectrum[_xPoints - x][_yPoints - i] * cos(-A)
            + _realSpectrum[_xPoints - x][_yPoints - i] * sin(-A);
    }
}

void Ocean::init_x_vertex_array(const int y, double* const vertices) const {
    for (int i = 0; i <= _xPoints; ++i) {
        vertices[3 * i] = (_sceneWidth / _xPoints) * i;
        vertices[3 * i + 2] = (_sceneHeight / _yPoints) * y;
    }
}

void Ocean::init_y_vertex_array(const int x, double* const vertices) const {
    for (int i = 0; i <= _yPoints; ++i) {
        vertices[3 * i] = (_sceneWidth / _xPoints) * x;
        vertices[3 * i + 2] = (_sceneHeight / _yPoints) * i;
    }
}

void Ocean::x_vertex_array(const int y, double* const vertices) const {
    for (int i = 0; i < _xPoints; ++i)
        vertices[3 * i + 1] = pow(-1, i + y) * _realTime[y][i];
    vertices[3 * _xPoints + 1] = pow(-1, _xPoints + y) * _realTime[y][_xPoints];
}

void Ocean::y_vertex_array(const int x, double* const vertices) const {
    for (int i = 0; i < _yPoints; ++i)
        vertices[3 * i + 1] = pow(-1, x + i) * _realTime[i][x];
    vertices[3 * _yPoints + 1] = pow(-1, x + _yPoints) * _realTime[0][x];
}