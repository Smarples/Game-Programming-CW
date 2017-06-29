// Simplified Renderer application for GP course
// Code is similar to the one in lab 1 but all the graphics sections were refactored into the Graphics Class.
// Extra improvements:
// Reduced OpenGL version from 4.5 to 3.3 to allow it to render in older laptops.
// Added Shapes library for rendering cubes, spheres and vectors.
// Added examples of matrix multiplication on Update.
// Added resize screen and keyboard callbacks.
// 
// Suggestions or extra help please do email me S.Padilla@hw.ac.uk
//
// Note: Do not forget to link the libraries correctly and add the GLEW DLL in your debug/release folder.

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

// VARIABLES
bool		running = true;

Graphics	myGraphics;		// Runing all the graphics in this object
Sphere		mySphere;
Cube		myCube;

float friction = 0.3f;
float gravity = 0.001f;
float speed = 0.0f;
float initspeed = 0.0f;
bool apply = true;
float top = 2.2f;
float t = 0.001f;			// Global variable for animation

glm::vec3 sposition = glm::vec3(0.0f, 2.2f, -6.0f);
glm::vec3 cposition = glm::vec3(0.0f, -2.0f, -6.0f);

int main()
{
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
	myCube.Load();
	mySphere.Load();
	mySphere.fillColor = glm::vec4(0.4f, 0.0f, 0.4f, 1.0f);	// You can change the shape fill colour, line colour or linewidth 
	myGraphics.SetOptimisations();		// Cull and depth testing
}

void update(double currentTime) {

	//apply used to stop operations on ball after stopped moving
	if (apply == true) {
		if (speed < 0.001 && speed > -0.001) {
			top = sposition[1];
		}
		//code for making the ball collide and bounce
		if ((sposition[1] - 0.49) < (cposition[1] + 0.5) && ((sposition[0] - 0.5 < cposition[0] + 0.5) || (sposition[0] + 0.5 < cposition[0] - 0.5)) && ((sposition[2] - 0.5 < cposition[2] + 0.5) || (sposition[2] + 0.5 < cposition[2] - 0.5))) {
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			mySphere.fillColor = glm::vec4(r, g, b, 1.0f);
			sposition[1] = cposition[1] + 1.0f;
			speed = -(speed - (speed*friction));
			float end = top - sposition[1];
			//makes ball stop bouncing
			if (end < 0.065) {
				apply = false;
			}
		}
		//code to make ball fall
		else {
			initspeed = speed;
			speed = (gravity * currentTime) + initspeed;
		}
		sposition[1] -= speed;
	}

	glm::mat4 mv_matrix_cube =
		glm::translate(cposition) *
		glm::rotate(t, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(t, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::mat4(1.0f);
	myCube.mv_matrix = mv_matrix_cube;
	myCube.proj_matrix = myGraphics.proj_matrix;


	// calculate Sphere movement
	glm::mat4 mv_matrix_sphere =
		glm::translate(sposition) *
		glm::rotate(-t, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(-t, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::mat4(1.0f);
	mySphere.mv_matrix = mv_matrix_sphere;
	mySphere.proj_matrix = myGraphics.proj_matrix;

}

void render(double currentTime) {
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();

	// Draw
	mySphere.Draw();
	myCube.Draw();

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

class vec3 {
public:
	vec3();
	vec3(float, float, float);
	bool operator==(vec3 rhs);
	vec3 operator+(vec3 rhs);
	vec3 operator-(vec3 rhs);
	vec3 operator*(vec3 rhs);
	vec3 operator/(vec3 rhs);
	vec3 operator+(float scalar);
	vec3 operator-(float scalar);
	vec3 operator*(float scalar);
	vec3 operator/(float scalar);
	vec3 cross(vec3 rhs);
	float dot(vec3 rhs);
	float length();

	float x;
	float y;
	float z;
};

vec3::vec3() {}

vec3::vec3(float, float, float) {}

bool vec3::operator==(vec3 rhs) {
	return(x == rhs.x && y == rhs.y && z == rhs.z);
}

vec3 vec3::operator+(vec3 rhs) {
	return vec3(x + rhs.x,
		y + rhs.y,
		z + rhs.z);
}

vec3 vec3::operator-(vec3 rhs) {
	return vec3(x - rhs.x,
		y - rhs.y,
		z - rhs.z);
}

vec3 vec3::operator*(vec3 rhs) {
	return vec3(x * rhs.x,
		y * rhs.y,
		z * rhs.z);
}

vec3 vec3::operator/(vec3 rhs) {
	return vec3(x / rhs.x,
		y / rhs.y,
		z / rhs.z);
}

vec3 vec3::operator/(float scalar) {
	return vec3(x / scalar,
		y / scalar,
		z / scalar);
}

vec3 vec3::operator*(float scalar) {
	return vec3(x * scalar,
		y * scalar,
		z * scalar);
}

vec3 vec3::operator+(float scalar) {
	return vec3(x + scalar,
		y + scalar,
		z + scalar);
}

vec3 vec3::operator-(float scalar) {
	return vec3(x - scalar,
		y - scalar,
		z - scalar);
}

float vec3::dot(vec3 rhs) {
	return (x * rhs.x +
		y * rhs.y +
		z * rhs.z);
}

vec3 vec3::cross(vec3 rhs) {
	return vec3(y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x);
}

float vec3::length() {
	return float(sqrt(x*x + y*y + z*z));
}


