#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.f, 1.f, 1.f);
	ambientIntensity = 1.f;

	direction = glm::vec3(0.f, -1.f, 0.f);
	diffuseIntensity = 1.f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientStrength,
			 GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseStrength)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = ambientStrength;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = diffuseStrength;
}

Light::~Light()
{
}

void Light::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
					 GLint diffuseIntensityLocation, GLint directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
