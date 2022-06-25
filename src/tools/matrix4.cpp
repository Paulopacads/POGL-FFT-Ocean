#include "matrix4.hpp"

Matrix4::Matrix4() {}

Matrix4::Matrix4(const float content[16]) {
    for (size_t i = 0; i < 16; ++i)
        _content[i] = content[i];
}

void Matrix4::operator*=(const Matrix4& rhs) {
    float tmp[4] = { 0 };
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j)
            for (size_t k = 0; k < 4; ++k)
                tmp[j] += _content[i * 4 + k] * rhs[k * 4 + j];

        for (size_t j = 0; j < 4; ++j)
            _content[i * 4 + j] = tmp[j];
    }
}

float Matrix4::operator[](const size_t i) const {
    return _content[i];
}

float& Matrix4::operator[](const size_t i) {
    return _content[i];
}

float *Matrix4::get_ptr() {
    return _content;
}

std::ostream& operator<<(std::ostream& out, const Matrix4& mat) {
    out << "Matrix: {\n"
        << mat[0]  << ' ' << mat[4]  << ' ' << mat[8]  << ' ' << mat[12]  << '\n'
        << mat[1]  << ' ' << mat[5]  << ' ' << mat[9]  << ' ' << mat[13]  << '\n'
        << mat[2]  << ' ' << mat[6]  << ' ' << mat[10] << ' ' << mat[14] << '\n'
        << mat[3] << ' ' << mat[7] << ' ' << mat[11] << ' ' << mat[15] << '\n'
        << '}';
    return out;
}
