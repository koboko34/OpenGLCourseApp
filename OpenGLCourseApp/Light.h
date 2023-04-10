#pragma once

#include <GL/glew.h>

#include <GLM/glm.hpp>

class Light
{
public:

	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientStrength,
		  GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseStrength);
	~Light();

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
				  GLint diffuseIntensityLocation, GLint directionLocation);

private:

	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;

};

