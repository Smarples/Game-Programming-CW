#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "Particle.h"

class Emitter {
public:
	Emitter();
	~Emitter();

	void Startup();
	void render();
	void Update(double currentTime, Graphics myGraphics);

	Particle myParticles[250];

};