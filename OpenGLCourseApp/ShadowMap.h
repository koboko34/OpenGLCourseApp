#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);

protected:
	GLuint FBO, shadowMap;
	GLuint shadowWidth, shadowHeight;
};

