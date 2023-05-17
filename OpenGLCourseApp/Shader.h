#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "CommonValues.h"

class Shader
{
public:
	
	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	GLuint GetProjectionLocation() const { return uniformProjection; }
	GLuint GetModelLocation() const { return uniformModel; }
	GLuint GetViewLocation() const { return uniformView; }
	GLuint GetEyePositionLocation() const { return uniformEyePosition; }
	GLuint GetAmbientColourLocation() const { return uniformDirectionalLight.uniformColour; }
	GLuint GetAmbientIntensityLocation() const { return uniformDirectionalLight.uniformAmbientIntensity; }
	GLuint GetDirectionLocation() const { return uniformDirectionalLight.uniformDirection; }
	GLuint GetDiffuseIntensityLocation() const { return uniformDirectionalLight.uniformDiffuseIntensity; }
	GLuint GetShininessLocation() const { return uniformShininess; }
	GLuint GetSpecularIntensityLocation() const { return uniformSpecularIntensity; }

	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLights(PointLight* pointLights, unsigned int lightCount);
	void SetSpotLights(SpotLight* spotLights, unsigned int lightCount);

	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

	void UseShader();
	void ClearShader();

private:

	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture, uniformDirectionalShadowMap,
		uniformDirectionalLightTransform;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	std::string ReadFile(const char* fileLocation);
};

