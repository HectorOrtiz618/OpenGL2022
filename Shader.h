#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "GL/glew.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"


class Shader
{
private:
	int pointLightCount;
	struct
	{
		GLuint uniformAmbiantColor;
		GLuint uniformAmbiantIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	}uniformDirectionalLight;

	struct
	{
		GLuint uniformAmbiantColor;
		GLuint uniformAmbiantIntensity;
		GLuint uniformDiffuseIntensity;
		
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	}uniformPointLight[MAX_POINT_LIGHTS];

	GLuint shaderID, uniformProjection, uniformModel,uniformView,uniformEyePosition, uniformSpecularIntensity, uniformShininess, uniformPointLightCount;
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
	
	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLight(PointLight* pLight, unsigned int pCount);
	void UseShader();
	void ClearShader();
	~Shader();
};

