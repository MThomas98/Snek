//** Import Libraries **//
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <string>
#include <algorithm>

//** Import classes **//
#include "Shader.h"
#include "Texture.h"
#include "Model.h"

//** Define window attributes **//
#define WIN_TITLE "CS310 Project"
#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000

//** Define game attributes **//
#define GRID_WIDTH 25
#define GRID_HEIGHT 25
#define TICK_RATE 2

using namespace std;

//** Graphics vars **//
Shader* shader;
Model* snakeHeadModel;
Model* snakeBodyModel;
Model* fruitModel;
Model* lightTileModel;
Model* darkTileModel;

//** Game vars **//
enum SnakeDirection { NORTH, EAST, SOUTH, WEST };
enum TileState { HEAD, BODY, EMPTY, FRUIT };
enum GameState { INIT, RUNNING, PAUSED, OVER };

int snakeX = 0;
int snakeY = 0;
int snakeLen = 3;
SnakeDirection snakeDir = NORTH;

int fruitX;
int fruitY;

TileState grid[GRID_WIDTH][GRID_HEIGHT];
GameState state = RUNNING;


/*---------------------*
*    Game functions    *
*----------------------*/

//** Run when game state is not "RUNNING" **//
void stopped() {
	// TODO: this
}

//** Run when game state is "RUNNING" **//
void running() {
	// Move the snake in the dir that it is moving
	switch (snakeDir) {
		case NORTH:
			snakeY++;
			break;
		case SOUTH:
			snakeY++;
			break;
		case EAST:
			snakeX++;
			break;
		case WEST:
			snakeX--;
			break;
	}

	// Collision check with wall
	if (snakeX < 0 || snakeX > GRID_WIDTH - 1 || snakeY < 0 || snakeY > GRID_WIDTH - 1) {
		state = OVER;
		return;
	}

	// Collision check with snake body
	if (grid[snakeX][snakeY] == BODY) {
		state = OVER;
		return;
	}

	// Collision check with snake body
	if (grid[snakeX][snakeY] == BODY) {
		state = OVER;
		return;
	}

	// Update the grid
	grid[snakeX][snakeY] = HEAD;
}

/*---------------------*
*    GLUT functions    *
*----------------------*/

//** The drawing function **//
void gDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This is the variable the transform matricies will be calculated to
	glm::mat4 model;

	// Draw out the grid
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_HEIGHT; y++) {
			// Draw the grid
			model = glm::translate(glm::mat4(1.0f), glm::vec3(-(GRID_WIDTH/2) + 0.5*(!(GRID_WIDTH % 2)), -(GRID_HEIGHT/2) + 0.5*(!(GRID_HEIGHT % 2)), 0.0f));
			model = glm::translate(model, glm::vec3(x, y, 0.0f));
			shader->setMat4f("model", model);

			// Alternate between dark and light
			if ((x + y) % 2 == 0)
				lightTileModel->draw();
			else
				darkTileModel->draw();

			// Draw what's at that location
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
			switch (grid[x][y]) {
				case HEAD:
					snakeHeadModel->draw();
					break;
				case BODY:
					snakeBodyModel->draw();
					break;
				case FRUIT:
					fruitModel->draw();
					break;
			}
		}
	}

	glutSwapBuffers();
}

//** The main game loop **//
void gLoop(int) {
	// Run the function that applies to the current game state
	switch (state) {
		case RUNNING:
			running();
			break;
		default:
			stopped();
	}

	printf("Head Loc: (%d, %d)\n", snakeX, snakeY);

	glutPostRedisplay();
	glutTimerFunc(1000/TICK_RATE, gLoop, 0);
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

	// Setup projection and view matricies for the shader
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -0.51*max(GRID_WIDTH, GRID_HEIGHT)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader->setMat4f("proj", proj);
	shader->setMat4f("view", view);

	// Load models and textures
	Texture darkGreen("img/green1.jpg");
	Texture lightGreen("img/green2.jpg");
	Texture whiteChecker("img/white_checker.jpg");
	Texture redChecker("img/red_checker.jpg");
	snakeHeadModel = new CubeModel(whiteChecker);
	snakeBodyModel = new CubeModel(whiteChecker);
	fruitModel = new CubeModel(redChecker);
	lightTileModel = new PlaneModel(lightGreen);
	darkTileModel = new PlaneModel(darkGreen);

	// Print OpenGL debug info
	printf("Finished Graphics initialisation.\n\n");
	printf("OpenGL Version: %s\nGLEW Version: %s\n\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));

	// Create the grid
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_HEIGHT; y++)
			grid[x][y] = EMPTY;
	}

	// Set GLUT functions
	glutDisplayFunc(gDisplay);
	glutTimerFunc(1000/TICK_RATE, gLoop, 0);
	glutMainLoop();

	return 0;
}
