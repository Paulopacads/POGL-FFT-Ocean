#pragma once

#include "ocean/philipps/philipps_spectrum.hpp"

#include <vector>

class Height {
    public:
        static double gaussian();
    
        Height(const int, const int);
        ~Height() {}
    
        double operator()();
    
        void init_fonctor(const int);
        void generate_philipps(Philipps* const);
     
    private:
        const int _xPoints;
        const int _yPoints;
        std::vector<std::vector<double>> _philippsSpectrums;
        int _currentX;
        int _currentY;
};