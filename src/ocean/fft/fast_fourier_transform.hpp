#pragma once

#include <cmath>
#include <vector>

class FFT {
    private:
        const int n;
        const int p;

        std::vector<double>* r_values;
        std::vector<double>* i_values;

        void sort();
    
    public:
        FFT(const int n, std::vector<double>* const r_values, std::vector<double>* const i_values)
            : n(n)
            , p(log2(n))
            , r_values(r_values)
            , i_values(i_values)
        {}
        
        void compute();
};