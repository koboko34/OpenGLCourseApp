#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.f, 1.f, 1.f);
	ambientIntensity = 1.f;
	diffuseIntensity = 1.f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientStrength, GLfloat diffuseStrength)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = ambientStrength;
	diffuseIntensity = diffuseStrength;
}

Light::~Light()
{
}
