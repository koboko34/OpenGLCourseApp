#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

class Texture
{
public:

	Texture();
	Texture(char* fileLoc);
	Texture(const char* fileLoc);
	~Texture();

	bool LoadTexture();
	bool LoadTextureA();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int height, width, bitDepth;

	char* fileLocation;
};

