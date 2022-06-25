#include "gl_utils.hpp"

#include <cmath>

Matrix4 lookAt(const float &eyeX, const float &eyeY,
            const float &eyeZ, const float &centerX,
            const float &centerY, const float &centerZ,
            const float &upX, const float &upY,
            const float &upZ) {

    Matrix4 mat = Matrix4();
    float forward[3];
    float side[3];
    float up[3];

    forward[0] = centerX - eyeX;
    forward[1] = centerY - eyeY;
    forward[2] = centerZ - eyeZ;

    float tmp = 0;
    for (size_t i = 0; i < 3; ++i)
        tmp += forward[i] * forward[i];

    tmp = std::sqrt(tmp);
    for (size_t i = 0; i < 3; ++i)
        forward[i] /= tmp;

    side[0] = forward[1] * upZ - forward[2] * upY;
    side[1] = forward[2] * upX - forward[0] * upZ;
    side[2] = forward[0] * upY - forward[1] * upX;

    tmp = 0;
    for (size_t i = 0; i < 3; ++i)
        tmp += side[i] * side[i];

    tmp = std::sqrt(tmp);
    for (size_t i = 0; i < 3; ++i)
        side[i] /= tmp;

    up[0] = side[1] * forward[2] - side[2] * forward[1];
    up[1] = side[2] * forward[0] - side[0] * forward[2];
    up[2] = side[0] * forward[1] - side[1] * forward[0];

    mat[0] = side[0];
    mat[4] = side[1];
    mat[8] = side[2];
    mat[12] = -(eyeX * side[0] + eyeY * side[1] + eyeZ * side[2]);

    mat[1] = up[0];
    mat[5] = up[1];
    mat[9] = up[2];
    mat[13] = -(eyeX * up[0] + eyeY * up[1] + eyeZ * up[2]);

    mat[2] = -forward[0];
    mat[6] = -forward[1];
    mat[10] = -forward[2];
    mat[14] = eyeX * forward[0] + eyeY * forward[1] + eyeZ * forward[2];

    mat[3] = 0;
    mat[7] = 0;
    mat[11] = 0;
    mat[15] = 1;

    std::cout << mat << std::endl;

    return mat;
}

Matrix4 frustum(float left, float right, float bottom,
                float top, float nearVal, float farVal) {

    Matrix4 mat = Matrix4();

    mat[0] = 2 * nearVal / (right - left);

    mat[5] = 2 * nearVal / (top - bottom);

    mat[8] = (right + left) / (right - left);
    mat[9] = (top + bottom) / (top - bottom);
    mat[10] = -(farVal + nearVal) / (farVal - nearVal);
    mat[11] = -1;

    mat[14] = -2 * farVal * nearVal / (farVal - nearVal);

    std::cout << mat << std::endl;

    return mat;
}
