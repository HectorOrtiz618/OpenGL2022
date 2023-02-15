#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "GL/glew.h"
class Shader
{
private:
	GLuint shaderID, uniformProjection, uniformModel,uniformView,uniformAmbiantIntensity,uniformAmbiantColor, uniformDiffuseIntensity,uniformDirection, uniformEyePosition, uniformSpecularIntensity, uniformShininess;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
public:
	Shader();
	
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFromFile(const char* fileLocation);
	
	GLuint GetProjectionLocation() const;
	GLuint GetModelLocation() const;
	GLuint GetViewLocation() const;
	GLuint GetAmbiantIntensity() const;
	GLuint GetAmbiantColor() const;
	GLuint GetDiffuseIntensity()const;
	GLuint GetDirectionLocation()const;
	GLuint GetEyePosition()const;
	GLuint GetSpecularIntensity()const;
	GLuint GetShininessLocation()const;
	void UseShader();
	void ClearShader();
	~Shader();
};

