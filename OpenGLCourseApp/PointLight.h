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

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
		GLint diffuseIntensityLocation, GLint positionLocation,
		GLint constantLocation, GLint linearLocation, GLint exponentLocation);

private:

	glm::vec3 position;

	GLfloat constant, linear, exponent;

};

