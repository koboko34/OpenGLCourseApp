#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
public:

	Texture();
	Texture(char* fileLoc);
	Texture(const char* fileLoc);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int height, width, bitDepth;

	char* fileLocation;
};
