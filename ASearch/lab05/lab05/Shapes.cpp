#include "shapes.h"
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

Shapes::Shapes() {

};

Shapes::~Shapes() {

}

void Shapes::LoadObj() {

	std::vector< glm::vec3 > obj_vertices;
	std::vector< unsigned int > vertexIndices;
	istringstream rawDataStream(rawData);
	string dataLine;  int linesDone = 0;

	while (std::getline(rawDataStream, dataLine)) {
		if (dataLine.find("v ") != string::npos) {	// does this line have a vector?
			glm::vec3 vertex;

			int foundStart = dataLine.find(" ");  int foundEnd = dataLine.find(" ", foundStart + 1);
			vertex.x = stof(dataLine.substr(foundStart, foundEnd - foundStart));

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			vertex.y = stof(dataLine.substr(foundStart, foundEnd - foundStart));

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			vertex.z = stof(dataLine.substr(foundStart, foundEnd - foundStart));

			obj_vertices.push_back(vertex);
		}
		else if (dataLine.find("f ") != string::npos) { // does this line defines a triangle face?
			string parts[3];

			int foundStart = dataLine.find(" ");  int foundEnd = dataLine.find(" ", foundStart + 1);
			parts[0] = dataLine.substr(foundStart + 1, foundEnd - foundStart - 1);

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			parts[1] = dataLine.substr(foundStart + 1, foundEnd - foundStart - 1);

			foundStart = foundEnd; foundEnd = dataLine.find(" ", foundStart + 1);
			parts[2] = dataLine.substr(foundStart + 1, foundEnd - foundStart - 1);

			for (int i = 0; i < 3; i++) {		// for each part

				vertexIndices.push_back(stoul(parts[i].substr(0, parts[i].find("/"))));

				int firstSlash = parts[i].find("/"); int secondSlash = parts[i].find("/", firstSlash + 1);

				if (firstSlash != (secondSlash + 1)) {	// there is texture coordinates.
														// add code for my texture coordintes here.
				}
			}
		}

		linesDone++;
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i += 3) {
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 0] - 1].x);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 0] - 1].y);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 0] - 1].z);

		vertexPositions.push_back(obj_vertices[vertexIndices[i + 1] - 1].x);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 1] - 1].y);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 1] - 1].z);

		vertexPositions.push_back(obj_vertices[vertexIndices[i + 2] - 1].x);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 2] - 1].y);
		vertexPositions.push_back(obj_vertices[vertexIndices[i + 2] - 1].z);
	}
}


void Shapes::Load() {
	static const char * vs_source[] = { R"(
#version 330 core

in vec4 position;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void){
	gl_Position = proj_matrix * mv_matrix * position;
}
)" };

	static const char * fs_source[] = { R"(
#version 330 core

uniform vec4 inColor;
out vec4 color;

void main(void){
	color = inColor;
}
)" };

	program = glCreateProgram();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);
	checkErrorShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);
	checkErrorShader(vs);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	mv_location = glGetUniformLocation(program, "mv_matrix");
	proj_location = glGetUniformLocation(program, "proj_matrix");
	color_location = glGetUniformLocation(program, "inColor");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		vertexPositions.size() * sizeof(GLfloat),
		&vertexPositions[0],
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glLinkProgram(0);	// unlink
	glDisableVertexAttribArray(0); // Disable
	glBindVertexArray(0);	// Unbind
}

void Shapes::Draw() {
	glUseProgram(program);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_matrix[0][0]);
	glUniformMatrix4fv(mv_location, 1, GL_FALSE, &mv_matrix[0][0]);

	glUniform4f(color_location, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size() / 3);

	glUniform4f(color_location, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  glLineWidth(lineWidth);
	glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size() / 3);
}


void Shapes::checkErrorShader(GLuint shader) {
	// Get log length
	GLint maxLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// Init a string for it
	std::vector<GLchar> errorLog(maxLength);

	if (maxLength > 1) {
		// Get the log file
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		cout << "--------------Shader compilation error-------------\n";
		cout << errorLog.data();
	}
}

Cube::Cube() {
	// Exported from Blender a cube by default (OBJ File)
	rawData = R"(
v 0.100000 -0.100000 -0.100000
v 0.100000 -0.100000 0.100000
v -0.100000 -0.100000 0.100000
v -0.100000 -0.100000 -0.100000
v 0.100000 0.100000 -0.099999
v 0.099999 0.100000 0.100000
v -0.100000 0.100000 0.100000
v -0.100000 0.100000 -0.100000
f 1 3 4
f 8 6 5
f 5 2 1
f 6 3 2
f 7 4 3
f 1 8 5
f 1 2 3
f 8 7 6
f 5 6 2
f 6 7 3
f 7 8 4
f 1 4 8)";

	LoadObj();
}

Cube::~Cube() {

}