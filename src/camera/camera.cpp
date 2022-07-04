#include "camera.hpp"

#include <GL/freeglut.h>
#include <cmath>

int Camera::getMouseX() {
    return _mouseX;
}

int Camera::getMouseY() {
    return _mouseY;
}

float Camera::getSightX() {
    return _X + sin(_theta) * sin(_psi);
}

float Camera::getSightY() {
    return _Y + cos(_psi);
}

float Camera::getSightZ() {
    return _Z + cos(_theta) * sin(_psi);
}

float Camera::getX() {
    return _X;
}

float Camera::getY() {
    return _Y;
}

float Camera::getZ() {
    return _Z;
}

void Camera::setKeyboard(int i, bool pressed) {
    _keys[i] = pressed;
    if (pressed && i == _keyMap[PAUSE]) {
        if (_pause = !_pause)
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
        else {
            glutSetCursor(GLUT_CURSOR_NONE);
        }
    }
}

void Camera::setMouse(int x, int y) {
    _mouseX = x;
    _mouseY = y;
}

void Camera::setX(float X) {
    _X = X;
}

void Camera::setY(float Y) {
    _Y = Y;
}

void Camera::setZ(float Z) {
    _Z = Z;
}

Camera::Camera(KEYBOARD keyboard, float X, float Y, float Z, float psi, float theta, float rotationSpeed, float translationSpeed, int windowWidth, int windowHeight)
    : _keyboard(keyboard)
    , _X(X)
    , _Y(Y)
    , _Z(Z)
    , _mouseX(windowWidth / 2)
    , _mouseY(windowHeight / 2)
    , _psi(psi)
    , _theta(theta)
    , _rotationSpeed(rotationSpeed)
    , _translationSpeed(translationSpeed)
    , _time(0) {
    _keyMap[PAUSE] = 'p';

    switch(keyboard) {
    case AZERTY:
        _keyMap[FORWARD] = 'z';
        _keyMap[BACKWARD] = 's';
        _keyMap[LEFT] = 'q';
        _keyMap[RIGHT] = 'd';
        break;

    default:
        _keyMap[FORWARD] = 'w';
        _keyMap[BACKWARD] = 's';
        _keyMap[LEFT] = 'a';
        _keyMap[RIGHT] = 'd';
        break;
    }
}

bool Camera::rotation(int x, int y) {
    if (!_pause && (x - _mouseX || y - _mouseY)) {
        _theta -= (x - _mouseX) * _rotationSpeed;
        _psi += (y - _mouseY) * _rotationSpeed;

        if(_psi <= 0.1)
            _psi = 0.1;

        else if(_psi >= 0.95 * M_PI)
            _psi = 0.95 * M_PI;

        _mouseX = x;
        _mouseY = y;
        return true;
    }
    return false;
}

void Camera::translation() {
    float t = glutGet(GLUT_ELAPSED_TIME) - _time;
    _time = glutGet(GLUT_ELAPSED_TIME);

    if (_pause)
        return;

    if(_keys[_keyMap[FORWARD]]) {
        _X += sin(_theta) * sin(_psi) * _translationSpeed * t;
        _Y += cos(_psi) * _translationSpeed * t;
        _Z += cos(_theta) * sin(_psi) * _translationSpeed * t;
    }

    if(_keys[_keyMap[BACKWARD]]) {
        _X -= sin(_theta) * sin(_psi) * _translationSpeed * t;
        _Y -= cos(_psi)            * _translationSpeed * t;
        _Z -= cos(_theta) * sin(_psi) * _translationSpeed * t;
    }

    if(_keys[_keyMap[LEFT]]) {
        _X -= sin(_theta - M_PI / 2) * sin(_psi) * _translationSpeed * t;
        _Z -= cos(_theta - M_PI / 2) * sin(_psi) * _translationSpeed * t;
    }

    if(_keys[_keyMap[RIGHT]]) {
        _X -= sin(_theta + M_PI / 2) * sin(_psi) * _translationSpeed * t;
        _Z -= cos(_theta + M_PI / 2) * sin(_psi) * _translationSpeed * t;
    }
}