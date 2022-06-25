#pragma once

#include "tools/gl_utils.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <GL/glew.h>

class Program {
public:
    Program();
    ~Program();

    static Program *make_program(std::string &vertex_shader_src,
                                    std::string &fragment_shader_src) {
        std::ifstream vertex_file(vertex_shader_src, std::ios::in);
        if (vertex_file.fail()) {
            std::cerr << "Fail opening vertex file.";
            return NULL;
        }

        std::ifstream fragment_file(fragment_shader_src, std::ios::in);
        if (fragment_file.fail()) {
            std::cerr << "Fail opening fragment file.";
            return NULL;
        }

        std::string line;

        std::string vertex_content = "";
        while (getline(vertex_file, line))
            vertex_content += line + '\n';
        vertex_content += '\0';

        std::string fragment_content = "";
        while (getline(fragment_file, line))
            fragment_content += line + '\n';
        fragment_content += '\0';

        int compile_status = GL_TRUE;
        uint *shader_id = (uint *)std::malloc(2 * sizeof(uint));
        char *vertex_ptr = (char *)std::malloc(vertex_content.length() * sizeof(char));
        char *fragment_ptr = (char *)std::malloc(fragment_content.length() * sizeof(char));
        vertex_content.copy(vertex_ptr,vertex_content.length());
        fragment_content.copy(fragment_ptr,fragment_content.length());

        shader_id[0] = glCreateShader(GL_VERTEX_SHADER);TEST_OPENGL_ERROR();
        shader_id[1] = glCreateShader(GL_FRAGMENT_SHADER);TEST_OPENGL_ERROR();

        glShaderSource(shader_id[0], 1, (const char **)&(vertex_ptr), 0);TEST_OPENGL_ERROR();
        glShaderSource(shader_id[1], 1, (const char **)&(fragment_ptr), 0);TEST_OPENGL_ERROR();

        for (int i = 0; i < 2; ++i) {
            glCompileShader(shader_id[i]);TEST_OPENGL_ERROR();
            glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);TEST_OPENGL_ERROR();
            if (compile_status != GL_TRUE) {
                int log_size;
                char *shader_log;
                glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);TEST_OPENGL_ERROR();
                shader_log = (char *)std::malloc(log_size + 1);

                if(shader_log != 0) {
                    glGetShaderInfoLog(shader_id[i], log_size, &log_size, shader_log);TEST_OPENGL_ERROR();
                    std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                    std::free(shader_log);
                }

                std::free(vertex_ptr);
                std::free(fragment_ptr);
                glDeleteShader(shader_id[0]);TEST_OPENGL_ERROR();
                glDeleteShader(shader_id[1]);TEST_OPENGL_ERROR();
                std::free(shader_id);
                return NULL;
            }
        }

        std::free(vertex_ptr);
        std::free(fragment_ptr);

        int link_status = GL_TRUE;
        Program *program = new Program();
        uint program_id = program->get_program_id();
        if (!program_id) {
            std::free(shader_id);
            return program;
        }

        program->set_shaders(shader_id, 2);

        for (int i = 0; i < 2; ++i) {
            glAttachShader(program_id, shader_id[i]);TEST_OPENGL_ERROR();
        }

        glLinkProgram(program_id);TEST_OPENGL_ERROR();
        glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);TEST_OPENGL_ERROR();

        if (link_status == GL_TRUE)
            return program;

        char *log = program->get_log();
        if (log == NULL) {
            delete program;
            return NULL;
        }

        std::cerr << log << std::endl;
        std::free(log);
        return program;
    }

    uint get_program_id();
    char *get_log();
    bool is_ready();
    void use();
    void set_shaders(uint *, uint);
    uint get_shader_id(uint);

private:
    uint _program_id;
    uint _shaders_size;
    uint *_shaders;
};
