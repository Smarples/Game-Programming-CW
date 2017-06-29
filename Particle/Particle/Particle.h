#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

class Particle {
	public: 
		Particle();
		~Particle();

		void CreateParticle(int l);
		void Draw();
		void Update(double currentTime, Graphics myGraphics);

		int timer;


		glm::vec3   position, colour, direction;
		float speed = 0.03f;
		Sphere     mySphere;

		
};