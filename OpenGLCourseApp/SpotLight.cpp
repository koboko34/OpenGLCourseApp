#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::normalize(glm::vec3(0.f, -1.f, 0.f));
	edge = 0.f;
	procEdge = cosf(glm::radians(edge));
	isOn = true;
}

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat near, GLfloat far,
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat ambientStrength, GLfloat diffuseStrength,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLfloat cons, GLfloat lin, GLfloat exp,
	GLfloat edg) : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, ambientStrength, diffuseStrength, xPos,yPos, zPos, cons, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	edge = edg;
	procEdge = cosf(glm::radians(edge));
	isOn = true;
}

SpotLight::~SpotLight()
{
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);

	if (isOn)
	{
		glUniform1f(ambientIntensityLocation, ambientIntensity);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}
	else
	{
		glUniform1f(ambientIntensityLocation, 0.f);
		glUniform1f(diffuseIntensityLocation, 0.f);
	}


	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}
