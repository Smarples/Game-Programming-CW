#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

#include "Graphics.h"
#include "Shapes.h"
#include "Particle.h"


Particle::Particle() {

};

Particle::~Particle() {

};

void Particle::CreateParticle(int l) {
	position = glm::vec3(0.0f, 0.0f, -6.0f);
	mySphere.Load();

	timer = 110 + (rand() % (int)(250 - 110));



	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (1.0f - (-1.0f)) + (-1.0f);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (1.0f - (-1.0f)) + (-1.0f);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (1.0f - (-1.0f)) + (-1.0f);

	mySphere.fillColor = glm::vec4(r, g, b, 1.0f);
	mySphere.lineColor = glm::vec4(r, g, b, 1.0f);

	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (1.0f - (-1.0f)) + (-1.0f);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (1.0f - (-1.0f)) + (-1.0f);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * (1.0f - (-1.0f)) + (-1.0f);

	direction = glm::vec3(x, y, z);
}

void Particle::Draw() {
	mySphere.Draw();
}

void Particle::Update(double currentTime, Graphics myGraphics) {

	glm::vec3 vel = direction * speed;

	position = position + vel;

	timer--;


	//Translations and scaling for sphere
	glm::mat4 mv_matrix_sphere =
		glm::translate(glm::vec3(position)) *
		glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4(1.0f);
	mySphere.mv_matrix = mv_matrix_sphere;
	mySphere.proj_matrix = myGraphics.proj_matrix;



}

