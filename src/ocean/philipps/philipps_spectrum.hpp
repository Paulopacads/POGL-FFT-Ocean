#pragma once

class Philipps {
    public:
        Philipps(const double, const double, const int, const int, const double, const int, const double, const double);
        ~Philipps() {}
    
        double operator()();
    
        void init_fonctor(const int);

    private:
        const double _sceneWidth;
        const double _sceneHeight;
        const int _xPoints;
        const int _yPoints;

        const double _windSpeed;
        const int _windForce;

        const double _minWaveSize;
        const double _waveHeight;

        int _currentX;
        int _currentY;
};