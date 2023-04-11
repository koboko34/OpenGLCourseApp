#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.f, 0.f, 0.f);
	constant = 1.f;
	linear = 0.f;
	exponent = 0.f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat ambientStrength, GLfloat diffuseStrength,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat cons, GLfloat lin, GLfloat exp) : Light(red, green, blue, ambientStrength, diffuseStrength)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = cons;
	linear = lin;
	exponent = exp;
}

PointLight::~PointLight()
{
}

void PointLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
	GLint diffuseIntensityLocation, GLint positionLocation,
	GLint constantLocation, GLint linearLocation, GLint exponentLocation)
{
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}
