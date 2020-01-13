//** Import Libraries **//
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <signal.h>

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
#define TICK_RATE 5

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
enum GameState { INIT, RUNNING, PAUSED, OVER, CLOSING };

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

	// Reset the view and proj for world drawing
	float orthoBound = 2*max(GRID_WIDTH, GRID_HEIGHT);
	glm::mat4 proj = glm::ortho(0.0f, orthoBound, 0.0f, orthoBound, -100.0f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader->setMat4f("view", view);
	shader->setMat4f("proj", proj);

	// This is the variable the transform matricies will be calculated to
	glm::mat4 model;

	// Draw out the grid
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_HEIGHT; y++) {
			// Draw the grid
			// model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.4*max(GRID_WIDTH, GRID_HEIGHT), 0.0f));
			model = glm::translate(glm::mat4(1.0f), glm::vec3(-(GRID_WIDTH/2) + 0.5*(!(GRID_WIDTH % 2)), -(GRID_HEIGHT/2) + 0.5*(!(GRID_HEIGHT % 2)), 0.0f));
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

	// Set the view and proj for UI drawing
	view = glm::mat4(1.0f);
	proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f);
	shader->setMat4f("view", view);
	shader->setMat4f("proj", proj);

	// // Draw the wasd compass
	// model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, 1.0f));
	// model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	// model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// shader->setMat4f("model", model);
	// compassModel->draw();

	glutSwapBuffers();
}

//** Function handling key events **//
void gKeyEvent(unsigned char key, int, int) {
	// TODO: Make this better
	// Change direction on WASD presses
	switch (key) {
		case 'w':
		case 'W':
			if (snakeDir != SOUTH && snakeDir != NORTH) nextSnakeDir = NORTH;
			else if (snakeDir == NORTH || snakeDir == SOUTH) nextSnakeDir = WEST;
			break;
		case 'a':
		case 'A':
			if (snakeDir != EAST) nextSnakeDir = WEST;
			break;
		case 's':
		case 'S':
			if (snakeDir != NORTH && snakeDir != SOUTH) nextSnakeDir = SOUTH;
			else if (snakeDir != WEST) nextSnakeDir = EAST;
			break;
		case 'd':
		case 'D':
			if (snakeDir != WEST && snakeDir != EAST) nextSnakeDir = EAST;
			else if (snakeDir == WEST) nextSnakeDir = NORTH;
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

	if (state == CLOSING) {
		printf("\n\n\n");
		exit(0);
	}

	glutPostRedisplay();
	glutTimerFunc(1000/TICK_RATE, gLoop, 0);
}


/*---------------------*
*    Init functions    *
*----------------------*/

void handleInt(int) {
	state = CLOSING;
}

//** Entry function **//
int main(int argc, char* argv[]) {
	// For fixing stdout buffering on windows
	#ifdef _WIN32
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	#endif

	// Serialise the randomiser
	srand(time(NULL));

	// Catch interrupts
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = handleInt;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

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

	// Put a fruit down
	grid[rand() % GRID_WIDTH][rand() % GRID_HEIGHT] = -1;

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
