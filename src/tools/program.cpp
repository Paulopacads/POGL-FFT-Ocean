#include "program.hpp"

Program::Program() {
    _program_id = glCreateProgram();TEST_OPENGL_ERROR();
}

Program::~Program() {
    if (_program_id != 0) {
        glDeleteProgram(_program_id);TEST_OPENGL_ERROR();
    }

    if (_shaders != NULL) {
        for (int i = 0; i < 2; ++i) {
            glDeleteShader(_shaders[i]);TEST_OPENGL_ERROR();
        }

        std::free(_shaders);
        _shaders = NULL;
        _shaders_size = 0;
    }
}

uint Program::get_program_id() {
    return _program_id;
}

char *Program::get_log() {
    int log_size;
    glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &log_size);TEST_OPENGL_ERROR();

    char *log = (char *) std::malloc(log_size + 1);
    if (log != NULL) {
        glGetProgramInfoLog(_program_id, log_size, &log_size, log);TEST_OPENGL_ERROR();
    }

    return log;
}

bool Program::is_ready() {
    return _program_id != 0;
}

void Program::use() {
    glUseProgram(_program_id);TEST_OPENGL_ERROR();
}

void Program::set_shaders(uint *shaders, uint size) {
    _shaders_size = size;
    _shaders = shaders;
}

uint Program::get_shader_id(uint index) {
    if (index < _shaders_size)
        return _shaders[index];
    return 0;
}
