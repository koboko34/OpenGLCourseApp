#pragma once

#include "Light.h"

class PointLight : public Light
{
public:

	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat ambientStrength, GLfloat diffuseStrength,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat cons, GLfloat lin, GLfloat exp);
	~PointLight();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

protected:

	glm::vec3 position;

	GLfloat constant, linear, exponent;

};

