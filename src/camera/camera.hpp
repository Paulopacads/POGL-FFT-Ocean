#pragma once

#include <map>

class Camera {

    public:
        enum KEYBOARD {AZERTY, QWERTY};
    
        Camera(KEYBOARD, float, float, float, float, float, float, float, int, int);
        ~Camera() {}
    
        int getMouseX();
        int getMouseY();
        float getSightX();
        float getSightY();
        float getSightZ();
        float getX();
        float getY();
        float getZ();

        void setKeyboard(int, bool);
        void setMouse(int, int);
        void setX(float);
        void setY(float);
        void setZ(float);
        
        bool rotation(int, int);
        void translation();
    
    private:
        enum ACTIONS {FORWARD, BACKWARD, LEFT, RIGHT, PAUSE};
    
        std::map<ACTIONS, int> _keyMap;
        KEYBOARD _keyboard;
        bool _keys[255] = { false };
        float _X;
        float _Y;
        float _Z;
        int _mouseX;
        int _mouseY;
        float _psi;
        float _theta;
        float _rotationSpeed;
        float _translationSpeed;
        int _time;
        bool _pause = false;
    
};