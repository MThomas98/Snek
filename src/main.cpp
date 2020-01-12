#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <string>

#include "Shader.h"
#include "Texture.h"
#include "Model.h"

#define WIN_TITLE "CS310 Project"
#define WIN_WIDTH 500
#define WIN_HEIGHT 500
#define FPS 60

using namespace std;

Shader* colorShader;
Shader* texShader;

Texture* sandTex;
Model* plane;
Model* cube;

/*---------------------*
*    GLUT functions    *
*----------------------*/

void gDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(0);

	texShader->use();

	// Set proj and view matricies
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	texShader->setMat4f("proj", proj);
	texShader->setMat4f("view", view);

	// Set plane's model matrix
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	texShader->setMat4f("model", model);

	// Draw the plane
	plane->draw();

	glutSwapBuffers();
}

void gLoop(int) {
	glutPostRedisplay();
	glutTimerFunc(1000/FPS, gLoop, 0);
}


/*---------------------*
*    Init functions    *
*----------------------*/

// Initialise OpenGL things
int init(int* argc, char** argv) {
	// Initialise GLUT window
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WIN_WIDTH)/2, (glutGet(GLUT_SCREEN_HEIGHT) - WIN_HEIGHT)/2);
	glutCreateWindow(WIN_TITLE);
	//glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	printf("\nInitialised GLUT.\n");

	// Initialise GLEW
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(err));
		return 0;
	}
	printf("Initialised GLEW.\n");

	// Create the shaders
	colorShader = new Shader("shaders/color.vert", "shaders/color.frag");
	texShader = new Shader("shaders/tex.vert", "shaders/tex.frag");
	printf("Compiled shaders.\n");

	// Create the textures
	sandTex = new Texture("tex/red_sand.png");

	// Create the models
	plane = new PlaneModel(*sandTex);
	cube = new CubeModel(*sandTex);

	printf("Finished initialisation.\n\n");
	printf("OpenGL Version: %s\nGLEW Version: %s\n\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));

	// Set GLUT functions
	glutDisplayFunc(gDisplay);
	glutTimerFunc(1000/FPS, gLoop, 0);
	glutMainLoop();

	return 1;
}

// Entry function
int main(int argc, char* argv[]) {
	// For fixing stdout buffering on windows
	#ifdef _WIN32
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	#endif

	// Check if initialisation is successful
	if (!init(&argc, argv)) {
		fprintf(stderr, "ERROR: Failed to initialise.\n");
		return -1;
	}

	return 0;
}
