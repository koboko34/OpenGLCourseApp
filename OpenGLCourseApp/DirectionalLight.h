#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:

	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
					GLfloat ambientStrength, GLfloat diffuseStrength,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);
	~DirectionalLight();

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
		GLint diffuseIntensityLocation, GLint directionLocation);

private:

	glm::vec3 direction;

};

