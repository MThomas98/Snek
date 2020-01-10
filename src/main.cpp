#include <GL/glew.h>
#include <GL/glut.h>

// #include <chrono>
#include <ctime>
#include "stdio.h"
#include "string.h"

#define WIN_TITLE "Snake"
#define WIN_WIDTH 1024
#define WIN_HEIGHT 1024
#define FPS 120

#define DEBUG true

using namespace std;

// TODO: Fix this for windows
// using namespace std::chrono;
// nanoseconds lastTime = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch());

void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void idle(int) {
	// TODO: Fix this for windows
    // nanoseconds curTime = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch());
    // nanoseconds deltans = curTime - lastTime;
	// float delta = deltans.count();
	// float fps = 1000000000/delta;
    // lastTime = curTime;
    // printf("\33[2K\33[2KFramerate: %.1ffps || Delta: %.0fms\r", fps, delta/1000000);
	// fflush(stdout);

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

    if (DEBUG) {
        printf("\nInitialised window.\n\nOpenGL Version: %s\nGLEW Version: %s\n\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));
		fflush(stdout);
	}

    glutMainLoop();

    return 1;
}

int main(int argc, char* argv[]) {
	#ifdef _WIN32
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	#endif

    if (!init(&argc, argv)) {
        fprintf(stderr, "ERROR: Failed to initialise.\n");
        return -1;
    }

    return 0;
}
