#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

#include "graphics.h"
#include "shapes.h"

// FUNCTIONS
void render(double currentTime);
void update(double currentTime);
void startup();
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool blocked(int x, int y);
//void up(int x, int y, int count);
//void down(int x, int y, int count);
//void left(int x, int y, int count);
//void right(int x, int y, int count);

void findFastestRoute(int spx, int spy, int epx, int epy);

glm::vec2 visited[400];
glm::vec2 possible[3];
glm::vec2 impossible[400];
glm::vec2 previous;
glm::vec2 dir[400];
int xdif;
int ydif;
glm::vec2 curpos;
bool db = false;
bool ub = false;
bool lb = false;
bool rb = false;
bool stuck = false;
int s = 0;


// VARIABLES
bool		running = true;

Graphics	myGraphics;		// Runing all the graphics in this object
							//Cube		myCube;
Cube		test3;

int INSTANCE_COUNT = 400;
const int oMax = 80;
int obstacle = 80;
int startposx;
int startposy;
int endposx;
int endposy;
int obstpla[oMax];
float t = 0.001f;			// Global variable for animation


int main()
{
	//Code for entering starting positions--------------------------------------------------------------------------------------------------------------
	cout << "Please enter a starting x and y position between (0,0) and (19,19)" << "\n";
	cin >> startposx;
	if (startposx <= 19 && startposx >= 0) {}
	else {
		while (startposx > 19 || startposx < 0) {
			cout << "Please enter a starting x Coordinate that is between 0 and 19" << "\n";
			cin >> startposx;
		}
		cout << "Please enter a starting y coordinate between 0 and 19" << "\n";
	}
	cin >> startposy;
	if (startposy <= 19 && startposy >= 0) {}
	else {
		while (startposy > 19 || startposy < 0) {
			cout << "Please enter a starting y Coordinate that is between 0 and 19" << "\n";
			cin >> startposy;
		}
	}
	//Code for entering starting positions--------------------------------------------------------------------------------------------------------------
	cout << "Please enter an ending x and y position between (0,0) and (19,19)" << "\n";
	cin >> endposx;
	if (endposx <= 19 && endposx >= 0) {}
	else {
		while (endposx > 19 || endposx < 0) {
			cout << "Please enter an ending x Coordinate that is between 0 and 19" << "\n";
			cin >> endposx;
		}
		cout << "Please enter an ending y coordinate between 0 and 19" << "\n";
	}
	cin >> endposy;
	if (endposy <= 19 && endposy >= 0) {}
	else {
		while (endposy > 19 || endposy < 0) {
			cout << "Please enter a ending y Coordinate that is between 0 and 19" << "\n";
			cin >> endposy;
		}
	}

	if (startposy == endposy && startposx == endposx) {
		while ((startposy == endposy && startposx == endposx) || (endposx > 19 || endposx < 0) || (endposy > 19 || endposy < 0) || (startposx > 19 || startposx < 0) || (startposy > 19 || startposy < 0)) {
			cout << "Please enter an ending x and y Coordinate between 0 and 19 that is different to the starting position" << "\n";
			cin >> endposx;
			cin >> endposy;
		}
	}


	int errorGraphics = myGraphics.Init();		// Launch window and graphics context
	if (errorGraphics) return 0;				//Close if something went wrong...

	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).

												// Mixed graphics and update functions - declared in main for simplicity.
	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);			// Set callback for resize
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);					// Set Callback for keys

																			// MAIN LOOP run until the window is closed
	do {
		double currentTime = glfwGetTime();		// retrieve timelapse
		glfwPollEvents();						// poll callbacks
		update(currentTime);					// update (physics, animation, structures, etc)
		render(currentTime);					// call render function.

		glfwSwapBuffers(myGraphics.window);		// swap buffers (avoid flickering and tearing)

		running &= (glfwGetKey(myGraphics.window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);	// exit if escape key pressed
		running &= (glfwWindowShouldClose(myGraphics.window) != GL_TRUE);
		//findFastestRoute(startposx, startposy, endposx, endposy);

	} while (running);

	myGraphics.endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

void startup() {

	// Calculate proj_matrix for the first time.
	myGraphics.aspect = (float)myGraphics.windowWidth / (float)myGraphics.windowHeight;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);

	// Load Geometry
	test3.Load();
	test3.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	test3.lineColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if (obstacle <= 0) {}
	else {
		for (int j = 0; j < oMax; j++) {
			obstpla[j] = rand() % INSTANCE_COUNT;
			obstacle--;
		}
	}


	//myCube.Load();
	//myCube.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	//myCube.lineColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	myGraphics.SetOptimisations();		// Cull and depth testing

}

void update(double currentTime) {
	/*
	glm::mat4 mv_matrix_cube =
	glm::translate(glm::vec3(-1.0f, -1.0f, -6.0f)) *
	glm::rotate(t, glm::vec3(0.0f, 0.0f, 1.0f)) *
	glm::rotate(t, glm::vec3(0.0f, 1.0f, 0.0f)) *
	glm::mat4(1.0f);
	myCube.mv_matrix = mv_matrix_cube;
	myCube.proj_matrix = myGraphics.proj_matrix;*/
}

void render(double currentTime) {
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();


	for (int i = 0; i < 20; i++) {
		for (int n = 0; n < 20; n++) {
			test3.fillColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			for (int o = 0; o < oMax; o++) {
				if ((i == startposy && n == startposx) || (i == endposy && n == endposx)) {
					test3.fillColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else if ((20 * n) + i == obstpla[o]) {
					test3.fillColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
			glm::mat4 mv_matrix_cube3 =
				glm::translate(glm::vec3((n*0.2f) - 2.0f, (i*0.2f) - 2.0f, -6.0f)) *
				glm::rotate(t, glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::rotate(t, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::mat4(1.0f);
			test3.mv_matrix = mv_matrix_cube3;
			test3.proj_matrix = myGraphics.proj_matrix;
			test3.Draw();
		}
	}

	// Draw
	//myCube.Draw();
	//test.Draw();
	//glUnmapBuffer(GL_ARRAY_BUFFER);

}

void onResizeCallback(GLFWwindow* window, int w, int h) {	// call everytime the window is resized
	myGraphics.windowWidth = w;
	myGraphics.windowHeight = h;

	myGraphics.aspect = (float)w / (float)h;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}

void findFastestRoute(int spx, int spy, int epx, int epy) {
	glm::vec2 endpos = glm::vec2(epx, epy);
	curpos = glm::vec2(spx, spy);
	dir[0] = glm::vec2(0,0);
	visited[0] = curpos;
	int i = 1;

	//loops until it finds the end
	while (curpos != endpos) {
		//initialises the possible grid spaces to impossible values
		for (int j = 0; j < 4; j++) {
			possible[j] = glm::vec2(-6, -6);
		}
		//find difference between current position to end position
		xdif = curpos[0] - endpos[0];
		ydif = curpos[1] - endpos[1];

		//finds which paths are blocked
		ub = blocked(curpos[0], curpos[1] + 1);
		db = blocked(curpos[0], curpos[1] - 1);
		rb = blocked(curpos[0] + 1, curpos[1]);
		lb = blocked(curpos[0] - 1, curpos[1]);

		//if paths are not blocked add them to possible movements
		if (ub != true) {
			possible[0] = glm::vec2(curpos[0], curpos[1] + 1);
		}
		if (db != true) {
			possible[1] = glm::vec2(curpos[0], curpos[1] - 1);
		}
		if (lb != true) {
			possible[2] = glm::vec2(curpos[0] - 1, curpos[1]);
		}
		if (rb != true) {
			possible[3] = glm::vec2(curpos[0] + 1, curpos[1]);
		}

		//if current position is the same as previous then you are stuck and need to figure out a way out
		for (int j = 0; j < 4; j++) {
			if (curpos == possible[j]) {
				stuck = true;
			}
			else if (possible[j] == previous) {
				impossible[s] = previous;
				s++;
			}
		}

		//loops through all possible movements to find best movement
		for (int j = 0; j < 4; j++) {
			if (possible[j] != glm::vec2(-6, -6)) {
				glm::vec2 tempdif = possible[i] - (endpos[0], endpos[1]);
				//if the curpos is stuck then will add it to impossible array to avoid going back there
				if (stuck == true) {
					impossible[s] = curpos;
					s++;
					stuck = false;
				}
				//if a possible action is impossible then dont go to it
				for (int k = 0; k <= s; k++) {
					if (possible[j] == impossible[k]) {
						possible[j] = glm::vec2(-6, -6);
					}
				}
				//tries to take the best action available 
				if (tempdif[1] < ydif) {
					previous = curpos;
					curpos = possible[j];
					dir[i] = tempdif;
					visited[i] = curpos;
					i++;
					break;
				}else if (tempdif[1] < ydif) {
					previous = curpos;
					curpos = possible[j];
					dir[i] = tempdif;
					visited[i] = curpos;
					i++;
					break;
				}
				//takes another movement if the best action is not available
				else if (tempdif[0] > xdif) {
					previous = curpos;
					curpos = possible[j];
					dir[i] = tempdif;
					visited[i] = curpos;
					i++;
					break;
				}
				else if (tempdif[1] > ydif) {
					previous = curpos;
					curpos = possible[j];
					dir[i] = tempdif;
					visited[i] = curpos;
					i++;
					break;
				}
			}
		}

		if (xdif == 0 && ydif == 0) {
			cout << "yes";
			break;
		}
	}
	}

bool blocked(int x, int y) {
	for (int i = 0; i < oMax; i++) {
		if ((20 * y) + x == obstpla[i]) {
			return true;
		}
	}
	return false;
}