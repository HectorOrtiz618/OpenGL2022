#pragma once
#include "GL/glew.h"

#include"stb_image.h"
class Texture
{
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
public:
	Texture();
	Texture(const char* fileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

};

