#include <GL/glew.h>
#include <GL/glut.h>

#include <chrono>
#include <ctime>
#include "stdio.h"
#include "string.h"

#define WIN_TITLE "Snake"
#define WIN_WIDTH 1024
#define WIN_HEIGHT 1024
#define FPS 60

#define DEBUG true

using namespace std;
using namespace std::chrono;

static high_resolution_clock::time_point lastTime = high_resolution_clock::now();

void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void idle(int) {
    high_resolution_clock::time_point curTime = high_resolution_clock::now();
    duration<double, milli> delta = curTime - lastTime;
    lastTime = curTime;
    printf("Delta time: %d\n", delta.count());

    glutPostRedisplay();
    glutTimerFunc(1000/FPS, idle, 0);
}

int init(int* argc, char** argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WIN_WIDTH)/2, (glutGet(GLUT_SCREEN_HEIGHT) - WIN_HEIGHT)/2);
    glutCreateWindow("Snake");

    GLenum err = glewInit();
    if(err != GLEW_OK) {
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(err));
        return 0;
    }

    glutDisplayFunc(display);
    glutTimerFunc(1000/FPS, idle, 0);

    if (DEBUG)
        printf("\nInitialised window.\n\nOpenGL Version: %s\nGLEW Version: %s\n\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));

    glutMainLoop();

    return 1;
}

int main(int argc, char* argv[]) {
    if (!init(&argc, argv)) {
        fprintf(stderr, "ERROR: Failed to initialise.\n");
        return -1;
    }

    return 0;
}
