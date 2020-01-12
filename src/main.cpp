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

Shader* shader;

Texture* grassTex;
Model* plane;
Model* cube;

/*---------------------*
*    GLUT functions    *
*----------------------*/

const glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 100.0f);
const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

void gDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set plane's model matrix
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader->setMat4f("model", model);

	// Draw the plane
	grassTex->bind();
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

int main(int argc, char* argv[]) {
	// For fixing stdout buffering on windows
	#ifdef _WIN32
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	#endif

	// Initialise GLUT window
	glutInit(&argc, argv);
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
		return -1;
	}
	printf("Initialised GLEW.\n");

	// Create the shader
	shader = new Shader("shaders/shader.vert", "shaders/shader.frag");
	shader->use();
	printf("Compiled shaders.\n");

	// Setup proj and view matricies and put them in to the shader
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	shader->setMat4f("proj", proj);
	shader->setMat4f("view", proj);

	// Crete models and textures
	grassTex = new Texture("img/grass.jpg");
	plane = new PlaneModel(*grassTex);
	cube = new CubeModel(*grassTex);

	// Print OpenGL debug info
	printf("Finished initialisation.\n\n");
	printf("OpenGL Version: %s\nGLEW Version: %s\n\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));

	// Set GLUT functions
	glutDisplayFunc(gDisplay);
	glutTimerFunc(1000/FPS, gLoop, 0);
	glutMainLoop();

	return 0;
}
