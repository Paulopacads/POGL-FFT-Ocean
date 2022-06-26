#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "camera/camera.hpp"
#include "tools/gl_utils.hpp"
#include "tools/option_parsing.hpp"
#include "tools/program.hpp"
#include "object_vbo.hpp"

Camera *camera;
Program *program;
uint vao_id;
struct options options;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id);TEST_OPENGL_ERROR();
    camera->translation();

    int world_camera_location = glGetUniformLocation(program->get_program_id(), "world_camera");TEST_OPENGL_ERROR();
    if (world_camera_location != -1) {
        Matrix4 matrix = lookAt(camera->getX(), camera->getY(), camera->getZ(), camera->getSightX(), camera->getSightY(), camera->getSightZ(), 0, 1, 0);
        glUniformMatrix4fv(world_camera_location, 1, false, matrix.get_ptr());TEST_OPENGL_ERROR();
    }

    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glutSwapBuffers();TEST_OPENGL_ERROR();
    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    options.rx = width;
    options.ry = height;

    int projection_location = glGetUniformLocation(program->get_program_id(), "projection");TEST_OPENGL_ERROR();

    if (projection_location != -1) {
        float i = width;
        float j = height;
        float tmp = sqrt(i * i + j * j) / 2;
        i /= tmp;
        j /= tmp;
        Matrix4 matrix = frustum(-i, i, -j, j, 2, 300);
        glUniformMatrix4fv(projection_location, 1, false, matrix.get_ptr());TEST_OPENGL_ERROR();
    }
}

bool init_glut(struct options& opt) {
    int argc = 1;
    char *argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);TEST_OPENGL_ERROR();
    glutInitContextVersion(4, 5);TEST_OPENGL_ERROR();
    glutInitContextProfile(GLUT_CORE_PROFILE);TEST_OPENGL_ERROR();
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);TEST_OPENGL_ERROR();
    glutInitWindowSize(opt.rx, opt.ry);TEST_OPENGL_ERROR();
    glutInitWindowPosition (0, 0);TEST_OPENGL_ERROR();
    glutCreateWindow("POGL");TEST_OPENGL_ERROR();
    glEnable(GL_MULTISAMPLE);

    glutDisplayFunc(display);TEST_OPENGL_ERROR();
    glutReshapeFunc(reshape);TEST_OPENGL_ERROR();

    return true;

}

bool init_glew() {
    return glewInit() == GLEW_OK;
}

bool init_GL() {
    glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);TEST_OPENGL_ERROR();
    glEnable(GL_CULL_FACE);TEST_OPENGL_ERROR();
    glClearColor(.2, .2, .2, 1.);TEST_OPENGL_ERROR();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    return true;
}

bool init_object() {
    int color_location = glGetUniformLocation(program->get_program_id(), "color");TEST_OPENGL_ERROR();

    if (color_location == -1)
        return false;

    glUniform3f(color_location, 1., 1., 1.);TEST_OPENGL_ERROR();

    uint vbo_id[1];

    int vertex_position = glGetAttribLocation(program->get_program_id(), "position");TEST_OPENGL_ERROR();

    if (vertex_position == -1)
        return false;

    glGenVertexArrays(1, &vao_id);TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id);TEST_OPENGL_ERROR();

    glGenBuffers(1, vbo_id);TEST_OPENGL_ERROR();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float), vertex_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(vertex_position);TEST_OPENGL_ERROR();

    glBindVertexArray(0);TEST_OPENGL_ERROR();
    return true;
}

bool init_pov() {
    // int world_camera_location = glGetUniformLocation(program->get_program_id(), "world_camera");TEST_OPENGL_ERROR();

    // if (world_camera_location == -1)
    //     return false;

    // Matrix4 matrix = lookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);
    // glUniformMatrix4fv(world_camera_location, 1, false, matrix.get_ptr());TEST_OPENGL_ERROR();

    int projection_location = glGetUniformLocation(program->get_program_id(), "projection");TEST_OPENGL_ERROR();

    if (projection_location == -1)
        return false;

    float i = options.rx;
    float j = options.ry;
    float tmp = sqrt(i * i + j * j) / 2;
    i /= tmp;
    j /= tmp;
    Matrix4 matrix = frustum(-i, i, -j, j, 2, 300);
    glUniformMatrix4fv(projection_location, 1, false, matrix.get_ptr());TEST_OPENGL_ERROR();

    return true;
}

bool init_shaders() {
    std::string vertex_src = "vertex.shd";
    std::string fragment_src = "fragment.shd";
    program = Program::make_program(vertex_src, fragment_src);

    if (program == NULL)
        return false;

    if (!program->get_program_id()) {
        delete program;
        return false;
    }

    glUseProgram(program->get_program_id());TEST_OPENGL_ERROR();
    return true;
}

void mouse_move(int x, int y) {
    if (camera->rotation(x, y) && (x >= .75 * options.rx || x <= .25 * options.rx || y >= .75 * options.ry || y <= .25 * options.ry)) {
        glutWarpPointer(options.rx / 2, options.ry / 2);
        camera->setMouse(options.rx / 2, options.ry / 2);
    }
}
    
void kb_press(unsigned char key, int, int) {
    camera->setKeyboard(key, true);
}

void kb_release(unsigned char key, int, int) {
    camera->setKeyboard(key, false);
}

int init(struct options& opt) {
    options = opt;
    if (!init_glut(opt)) {
        std::cerr << "Having trouble initializing glut\n";
        return 1;
    }

    if (!init_glew()) {
        std::cerr << "Having trouble initializing glew\n";
        return 2;
    }

    if (!init_GL()) {
        std::cerr << "Having trouble initializing GL\n";
        return 3;
    }

    if (!init_shaders()) {
        std::cerr << "Having trouble initializing shaders\n";
        return 4;
    }

    if (!init_pov()) {
        std::cerr << "Having trouble initializing pov\n";
        return 5;
    }

    if (!init_object()) {
        std::cerr << "Having trouble initializing object\n";
        return 6;
    }

    camera = new Camera(Camera::KEYBOARD::QWERTY, 5, 5, 5, M_PI * .6, -M_PI * .75, .001, .05, opt.rx, opt.ry);
    glutPassiveMotionFunc(mouse_move);
    glutKeyboardFunc(kb_press);
    glutKeyboardUpFunc(kb_release);

    glutMainLoop();

    delete program;
    return 0;
}
