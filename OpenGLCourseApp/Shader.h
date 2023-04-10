#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

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
	GLuint GetAmbientColourLocation() const { return uniformAmbientColour; }
	GLuint GetAmbientIntensityLocation() const { return uniformAmbientIntensity; }
	GLuint GetDirectionLocation() const { return uniformDirection; }
	GLuint GetDiffuseIntensityLocation() const { return uniformDiffuseIntensity; }

	void UseShader();
	void ClearShader();

private:

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		   uniformAmbientColour, uniformAmbientIntensity, uniformDirection, uniformDiffuseIntensity;

	void CompileShaders(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	std::string ReadFile(const char* fileLocation);
};

