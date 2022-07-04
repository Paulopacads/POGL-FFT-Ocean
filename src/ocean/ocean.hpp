#pragma once

#include "fft/fast_fourier_transform.hpp"
#include "height/height_spectrum.hpp"

#include <vector>

class Ocean {
    public:
        Ocean(const double, const double, const int, const int, const double);
        ~Ocean();
    
        const int get_scene_width();
        const int get_scene_height();
        const int get_x_points();
        const int get_y_points();
    
        void generate_height(Height* const);
        void operator()();
        void init_x_vertex_array(const int, double* const) const;
        void init_y_vertex_array(const int, double* const) const;
        void x_vertex_array(const int, double* const) const;
        void y_vertex_array(const int, double* const) const;
    
    private:
        void get_sine_amp(const int, const double, std::vector<double>* const, std::vector<double>* const) const;
    
        const double _sceneWidth;
        const double _sceneHeight;
        const int _xPoints;
        const int _yPoints;
        const double _motionFactor;
  
        std::vector<std::vector<double>> _realSpectrum;
        std::vector<std::vector<double>> _imagSpectrum;
    
        std::vector<std::vector<double>> _realFrequency;
        std::vector<std::vector<double>> _imagFrequency;
        std::vector<std::vector<double>> _realTime;
        std::vector<std::vector<double>> _imagTime;
    
        std::vector<FFT*> _xFFT;
        std::vector<FFT*> _yFFT;
    
    
};