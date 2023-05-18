#pragma once

#include <vector>

#include "OmniShadowMap.h"

#include "Light.h"

class PointLight : public Light
{
public:

	PointLight();
	PointLight(GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat ambientStrength, GLfloat diffuseStrength,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat cons, GLfloat lin, GLfloat exp);
	~PointLight();

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	glm::vec3 GetPosition() const { return position; }

	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat GetFarPlane() const { return farPlane; }

protected:

	glm::vec3 position;

	GLfloat constant, linear, exponent;

	GLfloat farPlane;

};

