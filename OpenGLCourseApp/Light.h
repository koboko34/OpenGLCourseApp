#pragma once

#include <GL/glew.h>

#include <GLM/glm.hpp>

class Light
{
public:

	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientStrength, GLfloat diffuseStrength);
	~Light();

protected:

	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

};

