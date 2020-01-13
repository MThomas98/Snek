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
Model* compassModel;

//** Game vars **//
enum SnakeDirection { NORTH, EAST, SOUTH, WEST };
enum GameState { INIT, RUNNING, PAUSED, OVER };

int snakeX = 0;
int snakeY = 0;
int snakeLen = 3;
SnakeDirection snakeDir = NORTH;
SnakeDirection nextSnakeDir = NORTH;

int fruitX;
int fruitY;

int grid[GRID_WIDTH][GRID_HEIGHT];
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
	snakeDir = nextSnakeDir;

	// Move the snake in the dir that it is moving
	switch (snakeDir) {
		case NORTH:
			snakeX++;
			break;
		case SOUTH:
			snakeX--;
			break;
		case EAST:
			snakeY--;
			break;
		case WEST:
			snakeY++;
			break;
	}

	// Collision check with wall
	if (snakeX < 0 || snakeX > GRID_WIDTH - 1 || snakeY < 0 || snakeY > GRID_WIDTH - 1) {
		state = OVER;
		return;
	}

	// Collision check with snake body
	if (grid[snakeX][snakeY] > 0) {
		state = OVER;
		return;
	}

	// Update all grid tyles
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_WIDTH; y++) {
			if (grid[x][y] > 0)
				grid[x][y]--;
		}
	}

	// Update the grid with the snake head location
	grid[snakeX][snakeY] = snakeLen;
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
			// model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.4*max(GRID_WIDTH, GRID_HEIGHT), 0.0f));
			model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-(GRID_WIDTH/2) + 0.5*(!(GRID_WIDTH % 2)), -(GRID_HEIGHT/2) + 0.5*(!(GRID_HEIGHT % 2)), 0.0f));
			model = glm::translate(model, glm::vec3(x, y, 0.0f));
			shader->setMat4f("model", model);

			// Alternate between dark and light
			if ((x + y) % 2 == 0)
				lightTileModel->draw();
			else
				darkTileModel->draw();

			// Move the model matrix so the next object is drawn in front of the grid
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
			shader->setMat4f("model", model);

			// Draw what's at that location
			if (grid[x][y] == snakeLen)
				snakeHeadModel->draw();
			else if (grid[x][y] > 0)
				snakeBodyModel->draw();
			else if (grid[x][y] == -1)
				fruitModel->draw();
		}
	}

	// Draw the wasd compass
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5*max(GRID_WIDTH, GRID_HEIGHT), -1.5*max(GRID_WIDTH, GRID_HEIGHT), 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
	shader->setMat4f("model", model);
	compassModel->draw();

	glutSwapBuffers();
}

//** Function handling key events **//
void gKeyEvent(unsigned char key, int, int) {
	// Change direction on WASD presses
	switch (key) {
		case 'w':
		case 'W':
			if (snakeDir != SOUTH) nextSnakeDir = NORTH;
			break;
		case 'a':
		case 'A':
			if (snakeDir != EAST) nextSnakeDir = WEST;
			break;
		case 's':
		case 'S':
			if (snakeDir != NORTH) nextSnakeDir = SOUTH;
			break;
		case 'd':
		case 'D':
			if (snakeDir != WEST) nextSnakeDir = EAST;
	}
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

	//** Print debug information **/
	// Get the string for the direction
	string dirString;
	switch (snakeDir) {
		case NORTH:
			dirString = "NORTH";
			break;
		case EAST:
			dirString = "EAST";
			break;
		case SOUTH:
			dirString = "SOUTH";
			break;
		case WEST:
			dirString = "WEST";
	}

	// Print out the information
	if (state != OVER)
		printf("\033[KDirection: %s\n\033[KHead Location: (%d, %d)\n\033[2A\r", dirString.c_str(), snakeX, snakeY);
	else
		printf("\033[K\033[KDirection: %s\033[K\nHead Location: (%d, %d)\n\n\033[KGAME OVER!\n\n\033[5A\r", dirString.c_str(), snakeX, snakeY);

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
	// glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)WIN_WIDTH/WIN_HEIGHT, 0.1f, 100.0f);
	float orthoBound = 0.8*max(GRID_WIDTH, GRID_HEIGHT);
	glm::mat4 proj = glm::ortho(-orthoBound, orthoBound, -orthoBound, orthoBound, -100.0f, 100.0f);
	// glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -1.0f, -0.51*max(GRID_WIDTH, GRID_HEIGHT)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -22.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	shader->setMat4f("proj", proj);
	shader->setMat4f("view", view);

	// Load models and textures
	Texture darkGreen("img/green1.jpg");
	Texture lightGreen("img/green2.jpg");
	// Texture whiteChecker("img/white_checker.jpg");
	Texture redChecker("img/red_checker.jpg");
	Texture blackBox("img/black_box.jpg");
	Texture compass("img/compass.jpg");
	compassModel = new PlaneModel(compass);
	snakeHeadModel = new CubeModel(blackBox);
	snakeBodyModel = new CubeModel(blackBox);
	fruitModel = new CubeModel(redChecker);
	lightTileModel = new CubeModel(lightGreen);
	darkTileModel = new CubeModel(darkGreen);


	// Print OpenGL debug info
	printf("Finished Graphics initialisation.\n\n");
	printf("OpenGL Version: %s\nGLEW Version: %s\n\n", glGetString(GL_VERSION), glewGetString(GLEW_VERSION));

	// Create the grid
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_HEIGHT; y++)
			grid[x][y] = 0;
	}

	// Put the head in the middle of the grid
	grid[(int)GRID_WIDTH/2][(int)GRID_HEIGHT/2] = snakeLen;
	snakeX = (int)GRID_WIDTH/2;
	snakeY = (int)GRID_HEIGHT/2;

	// Set GLUT functions
	glutDisplayFunc(gDisplay);
	glutKeyboardFunc(gKeyEvent);
	glutTimerFunc(1000/TICK_RATE, gLoop, 0);
	glutMainLoop();

	return 0;
}
