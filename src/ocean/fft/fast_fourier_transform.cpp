#include "fast_fourier_transform.hpp"

#include <iostream>

void FFT::sort() {
    for (int i = 0, nc = n; i < p - 1; i++, nc /= 2) {
        std::vector<double> r_sort;
        std::vector<double> i_sort;

        r_sort.reserve(n);
        i_sort.reserve(n);

        std::vector<double> r1(nc / 2);
        std::vector<double> r2(nc / 2);
        std::vector<double> i1(nc / 2);
        std::vector<double> i2(nc / 2);

        std::vector<double>::iterator r_iter(r_sort.begin());
        std::vector<double>::iterator i_iter(i_sort.begin());
        for (int j = 0; j < n / nc; j++) {
            for (int k = 0; k < nc / 2; k++) {
                const double ii = 2 * k + j * nc;
                r1[k] = r_values->at(ii);
                r2[k] = r_values->at(ii + 1);
                i1[k] = i_values->at(ii);
                i2[k] = i_values->at(ii + 1);
            }
            r_sort.insert(r_iter, r1.begin(), r1.end());
            r_sort.insert(r_sort.end(), r2.begin(), r2.end());
            i_sort.insert(i_iter, i1.begin(), i1.end());
            i_sort.insert(i_sort.end(), i2.begin(), i2.end());

            r_iter = r_sort.end();
            i_iter = i_sort.end();
        }
        swap(r_sort, *r_values);
        swap(i_sort, *i_values);
    }
}

void FFT::compute() {
    sort();

    std::vector<double> rvc(n);
    std::vector<double> ivc(n);

    for (int i = 0, nc = n; i < p; i++, nc /= 2) {
        for (int j = 0; j < nc / 2; j++) {
            int powi = pow(2, i);
            for (int k = 0; k < powi; k++) {
                const int ii = k + 2 * j * powi;
                const int jj = ii + powi;
                const double pos = ii * M_PI / powi;

                const double pcos = cos(pos);
                const double psin = sin(pos);
                const double r1 = r_values->at(ii);
                const double i1 = i_values->at(ii);
                const double r2 = r_values->at(jj);
                const double i2 = i_values->at(jj);

                rvc[ii] = r1 + pcos * r2 - psin * i2;
                ivc[ii] = i1 + pcos * i2 + psin * r2;
                rvc[jj] = r1 - pcos * r2 + psin * i2;
                ivc[jj] = i1 - pcos * i2 - psin * r2;
            }
        }
        swap(rvc, *r_values);
        swap(ivc, *i_values);
    }
}