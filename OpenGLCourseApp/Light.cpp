#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.f, 1.f, 1.f);
	ambientIntensity = 1.f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientStrength)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = ambientStrength;
}

Light::~Light()
{
}

void Light::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}
