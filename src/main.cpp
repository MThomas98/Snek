#include <GL/glew.h>
#include <GL/glut.h>

#include "stdio.h"
#include "string.h"

#define WIN_TITLE "Snake"
#define WIN_WIDTH 1024
#define WIN_HEIGHT 1024

#define DEBUG true

int init(int* argc, char** argv);

int main(int argc, char* argv[]) {
    if (!init(&argc, argv)) {
        fprintf(stderr, "ERROR: Failed to initialise.\n");
        return -1;
    }

    return 0;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

int init(int* argc, char** argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Snake");

    GLenum err = glewInit();
    if(err != GLEW_OK) {
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(err));
        return 0;
    }

    if (DEBUG)
        printf("Initialised.\nOpenGL Version: %s\nGLEW Version: %s\n\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));

    glutDisplayFunc(display);
    glutMainLoop();

    return 1;
}
