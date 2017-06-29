#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

#include "particle_system.h"


bool RandomTexture::InitRandomTexture(unsigned int Size)
{
	glm::vec3* pRandomData = new glm::vec3[Size];

	for (unsigned int i = 0; i < Size; i++) {
		pRandomData[i].x = RandomFloat();
		pRandomData[i].y = RandomFloat();
		pRandomData[i].z = RandomFloat();
	}

	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_1D, m_textureObj);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, Size, 0.0f, GL_RGB, GL_FLOAT, pRandomData);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	delete[] pRandomData;

	return GLCheckError();
}