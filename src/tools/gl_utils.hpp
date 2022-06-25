#pragma once

#include "matrix4.hpp"
#include <iostream>
#include <GL/glew.h>

#define TEST_OPENGL_ERROR()   \
  do {                        \
    check_gl_error(__LINE__); \
  } while(0)

static void check_gl_error(int line) {
    GLenum err (glGetError());

    while(err!=GL_NO_ERROR) {
        std::cerr << "OpenGL ERROR! " << line << ": ";
        std::string temp = "";

        switch(err) {
        case GL_INVALID_OPERATION:
            temp="INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            temp="INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            temp="INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            temp="OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            temp="INVALID_FRAMEBUFFER_OPERATION";
            break;
        }

        std::cerr << "GL_" << temp << std::endl;
        err = glGetError();
    }
}


Matrix4 lookAt(const float&, const float&,
                const float&, const float&,
                const float&, const float&,
                const float&, const float&,
                const float&);

Matrix4 frustum(float, float, float,
                float, float, float);
