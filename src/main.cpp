#include <GL/glew.h>
#include <GL/glut.h>

#include "stdio.h"
#include "string.h"

#define WIN_TITLE "Snake"
#define WIN_WIDTH 512
#define WIN_HEIGHT 512

#define DEBUG true

int init(int* argc, char** argv);

int main(int argc, char* argv[]) {
    if (!init(&argc, argv)) {
        fprintf(stderr, "ERROR: Failed to initialise.\n");
        return -1;
    }

    return 0;
}

int init(int* argc, char** argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Snake");

    if (DEBUG)
        printf("Initialised.\n");

    return 1;
}
