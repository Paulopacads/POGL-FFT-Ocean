#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "camera/camera.hpp"
#include "ocean/ocean.hpp"
#include "tools/gl_utils.hpp"
#include "tools/option_parsing.hpp"
#include "tools/program.hpp"
#include "object_vbo.hpp"

Camera *camera;
Ocean *ocean;
Program *program;
uint vao_id;
struct options options;
std::vector<double *> oceanVerticiesX;
std::vector<double *> oceanVerticiesY;

void mouse_move(int x, int y) {
    if (camera->rotation(x, y)) { //  && (x >= .75 * options.rx || x <= .25 * options.rx || y >= .75 * options.ry || y <= .25 * options.ry)
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

void update_buffer() {
    GLushort indices[ocean->get_x_points() * ocean->get_y_points() * 6];
    int i = 0;

    // Triangles
    for(int y = 0; y < ocean->get_y_points(); y++) {
        for(int x = 0; x < ocean->get_x_points(); x++) {
            indices[i++] = y * 101 + x;
            indices[i++] = y * 101 + x + 1;
            indices[i++] = (y + 1) * 101 + x + 1;

            indices[i++] = y * 101 + x;
            indices[i++] = (y + 1) * 101 + x + 1;
            indices[i++] = (y + 1) * 101 + x;
        }
    }

    GLuint surface_ibo;
    glGenBuffers(1, &surface_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
}
    
void display_ocean() {
    (*ocean)();
    glColor3ub(82, 184, 255);TEST_OPENGL_ERROR();
    for(int i = 0; i < ocean->get_x_points(); ++i) {
        ocean->y_vertex_array(i, oceanVerticiesY[i]);
        glEnableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
        glVertexPointer(3, GL_DOUBLE, 0, oceanVerticiesY[i]);TEST_OPENGL_ERROR();
        glDrawArrays(GL_LINE_STRIP, 0, ocean->get_y_points() + 1);TEST_OPENGL_ERROR();
        glDisableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
    }
    for(int i = 0; i < ocean->get_y_points(); ++i) {
        ocean->x_vertex_array(i, oceanVerticiesX[i]);
        glEnableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
        glVertexPointer(3, GL_DOUBLE, 0, oceanVerticiesX[i]);TEST_OPENGL_ERROR();
        glDrawArrays(GL_LINE_STRIP, 0, ocean->get_x_points() + 1);TEST_OPENGL_ERROR();
        glDisableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
    }
    glColor3ub(0, 0, 0);TEST_OPENGL_ERROR();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    // glBindVertexArray(vao_id);TEST_OPENGL_ERROR();
    camera->translation();

    int world_camera_location = glGetUniformLocation(program->get_program_id(), "world_camera");TEST_OPENGL_ERROR();
    if (world_camera_location != -1) {
        Matrix4 matrix = lookAt(camera->getX(), camera->getY(), camera->getZ(), camera->getSightX(), camera->getSightY(), camera->getSightZ(), 0, 1, 0);
        glUniformMatrix4fv(world_camera_location, 1, false, matrix.get_ptr());TEST_OPENGL_ERROR();
    }

    display_ocean();

    // glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());TEST_OPENGL_ERROR();
    // glBindVertexArray(0);TEST_OPENGL_ERROR();
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
    // glutInitContextVersion(4, 5);TEST_OPENGL_ERROR();
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
    // glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);TEST_OPENGL_ERROR();
    // glEnable(GL_CULL_FACE);TEST_OPENGL_ERROR();
    glClearColor(.2, .2, .2, 1.);TEST_OPENGL_ERROR();
    // glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    // glPixelStorei(GL_PACK_ALIGNMENT,1);
    return true;
}

bool init_object() {
    ocean = new Ocean(350, 350, 128, 128, .6);

    Height height = Height(128, 128);
    Philipps philipps = Philipps(350, 350, 128, 128, 50, 2, .1, .0000038);
    height.generate_philipps(&philipps);
    ocean->generate_height(&height);

    for(int i = 0; i < ocean->get_y_points(); ++i)
        oceanVerticiesX.push_back(new double[3 * (ocean->get_x_points() + 1)]);

    for(int i = 0; i < ocean->get_x_points(); ++i)
        oceanVerticiesY.push_back(new double[3 * (ocean->get_y_points() + 1)]);

    for(int i = 0; i < ocean->get_x_points(); ++i)
        ocean->init_y_vertex_array(i, oceanVerticiesY[i]);

    for(int i = 0; i < ocean->get_y_points(); ++i)
        ocean->init_x_vertex_array(i, oceanVerticiesX[i]);

    int color_location = glGetUniformLocation(program->get_program_id(), "color");TEST_OPENGL_ERROR();

    if (color_location == -1)
        return false;

    glUniform3f(color_location, 1., 1., 1.);TEST_OPENGL_ERROR();

    // uint vbo_id[1];

    // int vertex_position = glGetAttribLocation(program->get_program_id(), "position");TEST_OPENGL_ERROR();

    // if (vertex_position == -1)
    //     return false;

    // glGenVertexArrays(1, &vao_id);TEST_OPENGL_ERROR();
    // glBindVertexArray(vao_id);TEST_OPENGL_ERROR();

    // glGenBuffers(1, vbo_id);TEST_OPENGL_ERROR();

    // glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);TEST_OPENGL_ERROR();
    // glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float), vertex_buffer_data.data(), GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    // glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
    // glEnableVertexAttribArray(vertex_position);TEST_OPENGL_ERROR();

    // glBindVertexArray(0);TEST_OPENGL_ERROR();
    return true;
}

bool init_pov() {
    float i = options.rx;
    float j = options.ry;
    camera = new Camera(Camera::KEYBOARD::QWERTY, 5, 5, 5, M_PI * .6, -M_PI * .75, .001, .05, i, j);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc(mouse_move);TEST_OPENGL_ERROR();
    glutKeyboardFunc(kb_press);TEST_OPENGL_ERROR();
    glutKeyboardUpFunc(kb_release);TEST_OPENGL_ERROR();

    int projection_location = glGetUniformLocation(program->get_program_id(), "projection");TEST_OPENGL_ERROR();

    if (projection_location == -1)
        return false;

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

int init(struct options& opt) {
    options = opt;
    if (!init_glut(opt)) {
        std::cerr << "Having trouble initializing glut\n";
        return 1;
    }
    std::cout << "init_glut: done\n";

    if (!init_glew()) {
        std::cerr << "Having trouble initializing glew\n";
        return 2;
    }
    std::cout << "init_glew: done\n";

    if (!init_GL()) {
        std::cerr << "Having trouble initializing GL\n";
        return 3;
    }
    std::cout << "init_GL: done\n";

    if (!init_shaders()) {
        std::cerr << "Having trouble initializing shaders\n";
        return 4;
    }
    std::cout << "init_shaders: done\n";

    if (!init_pov()) {
        std::cerr << "Having trouble initializing pov\n";
        return 5;
    }
    std::cout << "init_pov: done\n";

    if (!init_object()) {
        std::cerr << "Having trouble initializing object\n";
        return 6;
    }
    std::cout << "init_object: done\n";

    glutMainLoop();

    for(int i = 0; i < ocean->get_y_points(); ++i)
        delete[] oceanVerticiesX[i];
    for(int i = 0; i < ocean->get_x_points(); ++i)
        delete[] oceanVerticiesY[i];
    delete program;
    delete camera;
    delete ocean;
    return 0;
}
