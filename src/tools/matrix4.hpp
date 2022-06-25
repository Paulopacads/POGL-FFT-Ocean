#pragma once

#include <stdlib.h>
#include <ostream>

class Matrix4 {
public:
    Matrix4();
    Matrix4(const float[16]);
    void operator*=(const Matrix4&);
    float operator[](const size_t) const;
    float& operator[](const size_t);
    float *get_ptr();

    static Matrix4 identity() {
        Matrix4 mat = Matrix4();
        mat[0] = mat[5] = mat[10] = mat[15] = 1;
        return mat;
    }

private:
    float _content[16] = { 0 };
};

std::ostream& operator<<(std::ostream&, const Matrix4&);
