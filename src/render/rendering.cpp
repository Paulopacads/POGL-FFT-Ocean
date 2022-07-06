#include "rendering.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "camera/camera.hpp"
#include "ocean/ocean.hpp"
#include "tools/gl_utils.hpp"
#include "tools/option_parsing.hpp"
#include "tools/program.hpp"

Camera *camera;
Ocean *ocean;
Program *program;
struct options options;
std::vector<double *> oceanVerticiesX;
float *vertex_buffer;
float *normals_buffer;
float *faces_normals;
uint vao_id;
uint vbo_id[2];

bool init_glew (void) {
    return glewInit() == GLEW_OK;
}

void move_camera (void) {
    camera->translation();
    int world_camera_location = glGetUniformLocation(program->get_program_id(), "world_camera");TEST_OPENGL_ERROR();
    if (world_camera_location != -1) {
        Matrix4 matrix = lookAt(camera->getX(), camera->getY(), camera->getZ(), camera->getSightX(), camera->getSightY(), camera->getSightZ(), 0, 1, 0);
        glUniformMatrix4fv(world_camera_location, 1, false, matrix.get_ptr());TEST_OPENGL_ERROR();
    }
}

void compute_normal(const double ax, const double ay, const double az,
                        const double bx, const double by, const double bz,
                        const double cx, const double cy, const double cz, float *normal){
    double v1[3], v2[3];

    v1[0] = bx - ax;
    v1[1] = by - ay;
    v1[2] = bz - az;

    v2[0] = cx - ax;
    v2[1] = cy - ay;
    v2[2] = cz - az;

    normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void draw_ocean (void) {
    (*ocean)();

    for(int i = 0; i <= ocean->get_y_points(); ++i) {
        ocean->x_vertex_array(i, oceanVerticiesX[i]);
    }

    int curr = 0;
    int normals_curr = 0;
    for (int i = 0; i < ocean->get_x_points(); ++i) {
        for (int j = 0; j < ocean->get_y_points(); ++j) {
            // Face 1 Point 1
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * j];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * j + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * j + 2];
            // Face 1 Point 2
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1)];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 2];
            // Face 1 Point 3
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 2];

            compute_normal(vertex_buffer[curr - 9], vertex_buffer[curr - 8], vertex_buffer[curr - 7],
                            vertex_buffer[curr - 6], vertex_buffer[curr - 5], vertex_buffer[curr - 4],
                            vertex_buffer[curr - 3], vertex_buffer[curr - 2], vertex_buffer[curr - 1], (faces_normals + normals_curr));

            normals_curr += 3;

            // Face 2 Point 1
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * (j + 1)];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * (j + 1) + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * (j + 1) + 2];
            // Face 2 Point 2
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1)];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 2];
            // Face 2 Point 3
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 2];

            compute_normal(vertex_buffer[curr - 9], vertex_buffer[curr - 8], vertex_buffer[curr - 7],
                            vertex_buffer[curr - 6], vertex_buffer[curr - 5], vertex_buffer[curr - 4],
                            vertex_buffer[curr - 3], vertex_buffer[curr - 2], vertex_buffer[curr - 1], (faces_normals + normals_curr));

            normals_curr += 3;
        }
    }

    int color_location = glGetUniformLocation(program->get_program_id(), "color");TEST_OPENGL_ERROR();

    if (color_location != -1) {
        glUniform3f(color_location, .337, .706, 1.);TEST_OPENGL_ERROR();
    }

    // glEnableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
    // glVertexPointer(3, GL_FLOAT, 0, buffer);TEST_OPENGL_ERROR();

    // glDrawArrays(GL_TRIANGLES, 0, ocean->get_x_points() * ocean->get_y_points() * 6);TEST_OPENGL_ERROR();

    int vertex_position = glGetAttribLocation(program->get_program_id(), "position");TEST_OPENGL_ERROR();

    if (vertex_position == -1)
        return;

    glBindVertexArray(vao_id);TEST_OPENGL_ERROR();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, ocean->get_x_points() * ocean->get_y_points() * 18 * sizeof(float), vertex_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(vertex_position);TEST_OPENGL_ERROR();

    vertex_position = glGetAttribLocation(program->get_program_id(), "normal");TEST_OPENGL_ERROR();

    if (vertex_position == -1)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, ocean->get_x_points() * ocean->get_y_points() * 18 * sizeof(float), normals_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(vertex_position);TEST_OPENGL_ERROR();

    glDrawArrays(GL_TRIANGLES, 0, ocean->get_x_points() * ocean->get_y_points() * 18);TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();

    if (color_location != -1) {
        glUniform3f(color_location, 1., 1., 1.);TEST_OPENGL_ERROR();
    }
}

void display (void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    move_camera();
    draw_ocean();

    glutSwapBuffers();TEST_OPENGL_ERROR();
    glFlush();TEST_OPENGL_ERROR();
    glutPostRedisplay();TEST_OPENGL_ERROR();
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
        Matrix4 matrix = frustum(-i, i, -j, j, 1, 500);
        glUniformMatrix4fv(projection_location, 1, false, matrix.get_ptr());TEST_OPENGL_ERROR();
    }
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

bool init_shaders (void) {
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
    Philipps philipps = Philipps(350, 350, 128, 128, 50, 2, .1, .00038);
    height.generate_philipps(&philipps);
    ocean->generate_height(&height);

    for(int i = 0; i <= ocean->get_y_points(); ++i)
        oceanVerticiesX.push_back(new double[3 * (ocean->get_x_points() + 1)]);

    for(int i = 0; i <= ocean->get_y_points(); ++i)
        ocean->init_x_vertex_array(i, oceanVerticiesX[i]);

    vertex_buffer = (float *) malloc(128 * 128 * 18 * sizeof(float));
    normals_buffer = (float *) malloc(128 * 128 * 18 * sizeof(float));
    faces_normals = (float *) malloc(128 * 128 * 6 * sizeof(float));

    glGenVertexArrays(1, &vao_id);TEST_OPENGL_ERROR();
    glBindVertexArray(vao_id);TEST_OPENGL_ERROR();

    glGenBuffers(2, vbo_id);TEST_OPENGL_ERROR();

    int curr = 0;
    int normals_curr = 0;
    for (int i = 0; i < ocean->get_x_points(); ++i) {
        for (int j = 0; j < ocean->get_y_points(); ++j) {
            // Face 1 Point 1
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * j];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * j + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * j + 2];
            // Face 1 Point 2
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1)];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 2];
            // Face 1 Point 3
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 2];

            compute_normal(vertex_buffer[curr - 9], vertex_buffer[curr - 8], vertex_buffer[curr - 7],
                            vertex_buffer[curr - 6], vertex_buffer[curr - 5], vertex_buffer[curr - 4],
                            vertex_buffer[curr - 3], vertex_buffer[curr - 2], vertex_buffer[curr - 1], (faces_normals + normals_curr));

            normals_curr += 3;

            // Face 2 Point 1
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * (j + 1)];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * (j + 1) + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * (j + 1) + 2];
            // Face 2 Point 2
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1)];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i][3 * (j + 1) + 2];
            // Face 2 Point 3
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 1];
            vertex_buffer[curr++] = oceanVerticiesX[i + 1][3 * j + 2];

            compute_normal(vertex_buffer[curr - 9], vertex_buffer[curr - 8], vertex_buffer[curr - 7],
                            vertex_buffer[curr - 6], vertex_buffer[curr - 5], vertex_buffer[curr - 4],
                            vertex_buffer[curr - 3], vertex_buffer[curr - 2], vertex_buffer[curr - 1], (faces_normals + normals_curr));

            normals_curr += 3;
        }
    }

    // glEnableClientState(GL_VERTEX_ARRAY);TEST_OPENGL_ERROR();
    // glVertexPointer(3, GL_FLOAT, 0, buffer);TEST_OPENGL_ERROR();

    // glDrawArrays(GL_TRIANGLES, 0, ocean->get_x_points() * ocean->get_y_points() * 6);TEST_OPENGL_ERROR();

    int vertex_position = glGetAttribLocation(program->get_program_id(), "position");TEST_OPENGL_ERROR();
    int vertex_normal = glGetAttribLocation(program->get_program_id(), "normal");TEST_OPENGL_ERROR();

    if (vertex_position == -1) {
        std::cout << "no position\n";
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, ocean->get_x_points() * ocean->get_y_points() * 18 * sizeof(float), vertex_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(vertex_position);TEST_OPENGL_ERROR();

    if (vertex_normal == -1) {
        std::cout << "no normal\n";
        return false;
    }

    for (int i = 0; i < ocean->get_x_points() * ocean->get_y_points() * 18; ++i) {
        normals_buffer[i] = faces_normals[i / 3];
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, ocean->get_x_points() * ocean->get_y_points() * 18 * sizeof(float), normals_buffer, GL_STATIC_DRAW);TEST_OPENGL_ERROR();
    glVertexAttribPointer(vertex_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);TEST_OPENGL_ERROR();
    glEnableVertexAttribArray(vertex_normal);TEST_OPENGL_ERROR();

    glBindVertexArray(0);TEST_OPENGL_ERROR();

    // GLuint normalbuffer;
	// glGenBuffers(1, &normalbuffer);
	// glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	// glBufferData(GL_ARRAY_BUFFER, 128 * 128 * 6, normals_buffer, GL_STATIC_DRAW);
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(2);

    return true;
}

bool myInit (void) {
    glClearColor(0.08, 0.08, 0.1, 1.0);TEST_OPENGL_ERROR();
    return true;
}

int init (struct options& opt) {
    options = opt;
    if (!init_glut(opt))
        return 1;
    if (!init_glew())
        return 2;
    if (!myInit())
        return 3;
    if (!init_shaders())
        return 4;
    if (!init_pov())
        return 5;
    if (!init_object())
        return 6;
    glutMainLoop();

    free(vertex_buffer);
    free(normals_buffer);
    free(faces_normals);
    for(int i = 0; i < ocean->get_y_points(); ++i)
        delete[] oceanVerticiesX[i];
    delete camera;
    delete ocean;
    delete program;
    return 0;
}