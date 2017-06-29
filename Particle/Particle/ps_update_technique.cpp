#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>

#include "particle_system.h"


bool PSUpdateTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "ps_update.vs")) {
		return false;
	}

	if (!AddShader(GL_GEOMETRY_SHADER, "ps_update.gs")) {
		return false;
	}

	const GLchar* Varyings[4];
	Varyings[0] = "Type1";
	Varyings[1] = "Position1";
	Varyings[2] = "Velocity1";
	Varyings[3] = "Age1";

	glTransformFeedbackVaryings(m_shaderProg, 4, Varyings, GL_INTERLEAVED_ATTRIBS);

	if (!Finalize()) {
		return false;
	}

	m_deltaTimeMillisLocation = GetUniformLocation("gDeltaTimeMillis");
	m_randomTextureLocation = GetUniformLocation("gRandomTexture");
	m_timeLocation = GetUniformLocation("gTime");
	m_launcherLifetimeLocation = GetUniformLocation("gLauncherLifetime");
	m_shellLifetimeLocation = GetUniformLocation("gShellLifetime");
	m_secondaryShellLifetimeLocation = GetUniformLocation("gSecondaryShellLifetime");

	if (m_deltaTimeMillisLocation == INVALID_UNIFORM_LOCATION ||
		m_timeLocation == INVALID_UNIFORM_LOCATION ||
		m_randomTextureLocation == INVALID_UNIFORM_LOCATION) {
		m_launcherLifetimeLocation == INVALID_UNIFORM_LOCATION ||
			m_shellLifetimeLocation == INVALID_UNIFORM_LOCATION ||
			m_secondaryShellLifetimeLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}

	return true;
	}
}