#include "rendering.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "camera/camera.hpp"
#include "ocean/ocean.hpp"
#include "tools/gl_utils.hpp"
#include "tools/option_parsing.hpp"

Camera *camera;
Ocean *ocean;
struct options options;
std::vector<double *> oceanVerticiesX;
std::vector<double *> oceanVerticiesY;

void move_camera (void) {
    camera->translation();
    glMatrixMode(GL_MODELVIEW);
    Matrix4 matrix = lookAt(camera->getX(), camera->getY(), camera->getZ(), camera->getSightX(), camera->getSightY(), camera->getSightZ(), 0, 1, 0);
    glLoadMatrixf(matrix.get_ptr());TEST_OPENGL_ERROR();
}

void draw_ocean (void) {
    (*ocean)();
    glColor3ub(82, 184, 255);TEST_OPENGL_ERROR();
    for(int i = 0; i <= ocean->get_x_points(); ++i) {
        ocean->y_vertex_array(i, oceanVerticiesY[i]);
        glEnableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
        glVertexPointer(3, GL_DOUBLE, 0, oceanVerticiesY[i]);TEST_OPENGL_ERROR();
        glDrawArrays(GL_LINE_STRIP, 0, ocean->get_y_points() + 1);TEST_OPENGL_ERROR();
        glDisableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
    }
    for(int i = 0; i <= ocean->get_y_points(); ++i) {
        ocean->x_vertex_array(i, oceanVerticiesX[i]);
        glEnableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
        glVertexPointer(3, GL_DOUBLE, 0, oceanVerticiesX[i]);TEST_OPENGL_ERROR();
        glDrawArrays(GL_LINE_STRIP, 0, ocean->get_x_points() + 1);TEST_OPENGL_ERROR();
        glDisableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
    }
    glColor3ub(0, 0, 0);TEST_OPENGL_ERROR();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    options.rx = width;
    options.ry = height;

    float i = width;
    float j = height;
    float tmp = sqrt(i * i + j * j) / 2;
    i /= tmp;
    j /= tmp;

    glMatrixMode(GL_PROJECTION);
    Matrix4 matrix = frustum(-i, i, -j, j, 1, 500);
    glLoadMatrixf(matrix.get_ptr());TEST_OPENGL_ERROR();
}

void display (void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    move_camera();
    draw_ocean();

    glutSwapBuffers();TEST_OPENGL_ERROR();
    glFlush();
    glutPostRedisplay();
}

bool init_glew (void) {
    return glewInit() == GLEW_OK;
}

bool init_glut (struct options& opt) {
    int argc = 1;
    char *argv[1] = {(char*)"Something"};
    glutInit(&argc, argv);TEST_OPENGL_ERROR();
    glutInitContextProfile(GLUT_CORE_PROFILE);TEST_OPENGL_ERROR();
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);TEST_OPENGL_ERROR();
    glutInitWindowSize(opt.rx, opt.ry);TEST_OPENGL_ERROR();
    glutInitWindowPosition (0, 0);TEST_OPENGL_ERROR();
    glutCreateWindow("POGL");TEST_OPENGL_ERROR();
    glEnable(GL_MULTISAMPLE);TEST_OPENGL_ERROR();

    glutDisplayFunc(display);TEST_OPENGL_ERROR();
    glutReshapeFunc(reshape);TEST_OPENGL_ERROR();

    return true;
}

void mouse_move (int x, int y) {
    if (camera->rotation(x, y)) { //  && (x >= .75 * options.rx || x <= .25 * options.rx || y >= .75 * options.ry || y <= .25 * options.ry)
        glutWarpPointer(options.rx / 2, options.ry / 2);TEST_OPENGL_ERROR();
        camera->setMouse(options.rx / 2, options.ry / 2);
    }
}
    
void kb_press (unsigned char key, int, int) {
    camera->setKeyboard(key, true);
}

void kb_release (unsigned char key, int, int) {
    camera->setKeyboard(key, false);
}

bool init_pov (void) {
    float i = options.rx;
    float j = options.ry;
    camera = new Camera(Camera::KEYBOARD::QWERTY, 5, 5, 5, M_PI * .6, -M_PI * .75, .001, .05, i, j);
    glutSetCursor(GLUT_CURSOR_NONE);TEST_OPENGL_ERROR();
    glutPassiveMotionFunc(mouse_move);TEST_OPENGL_ERROR();
    glutKeyboardFunc(kb_press);TEST_OPENGL_ERROR();
    glutKeyboardUpFunc(kb_release);TEST_OPENGL_ERROR();
    return true;
}

bool init_object (void) {
    ocean = new Ocean(350, 350, 128, 128, .6);

    Height height = Height(128, 128);
    Philipps philipps = Philipps(350, 350, 128, 128, 50, 2, .1, .0000038);
    height.generate_philipps(&philipps);
    ocean->generate_height(&height);

    for(int i = 0; i <= ocean->get_y_points(); ++i)
        oceanVerticiesX.push_back(new double[3 * (ocean->get_x_points() + 1)]);

    for(int i = 0; i <= ocean->get_x_points(); ++i)
        oceanVerticiesY.push_back(new double[3 * (ocean->get_y_points() + 1)]);

    for(int i = 0; i <= ocean->get_x_points(); ++i)
        ocean->init_y_vertex_array(i, oceanVerticiesY[i]);

    for(int i = 0; i <= ocean->get_y_points(); ++i)
        ocean->init_x_vertex_array(i, oceanVerticiesX[i]);
    return true;
}

void myInit (void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);TEST_OPENGL_ERROR();
}

int init (struct options& opt) {
    options = opt;
    init_glew();
    init_glut(opt);
    myInit();
    init_pov();
    init_object();
    glutMainLoop();
    return 0;
}