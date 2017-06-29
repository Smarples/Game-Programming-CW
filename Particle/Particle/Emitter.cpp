#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

#include "graphics.h"
#include "shapes.h"
#include "Emitter.h"
#include "Particle.h"

Emitter::Emitter() {

};

Emitter::~Emitter() {

};

void Emitter::Startup() {

	for (int i = 0; i < 250; i++) {
		int lifespan = 110 + (rand() % (int)(250 - 110));
		myParticles[i].CreateParticle(lifespan);
	}

}

void Emitter::render() {
	for (int i = 0; i < 250; i++) {
		myParticles[i].Draw();
	}

}

void Emitter::Update(double currentTime, Graphics myGraphics) {
	for (int i = 0; i < 250; i++) {
		myParticles[i].Update(currentTime, myGraphics);
		if (myParticles[i].timer == 0) {
			myParticles[i].CreateParticle(200);
		}
	}



}
