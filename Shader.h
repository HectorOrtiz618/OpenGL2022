#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CommonValues.h"


class Shader
{
private:
	int pointLightCount, spotLightCount;
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

	struct
	{
		GLuint uniformAmbiantColor;
		GLuint uniformAmbiantIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
		GLuint uniformDirection;
		GLuint uniformEdge;
	}uniformSpotLight[MAX_SPOT_LIGHTS];

	GLuint shaderID, uniformProjection, uniformModel,uniformView,uniformEyePosition, uniformSpecularIntensity, uniformShininess, uniformPointLightCount, uniformSpotLightCount, uniformTexture, uniformDirectionalLightTransform, uniformDirectionalShadowMap, uniformOmniLightPos, uniformFarPlane;
	GLuint uniformLightMatrices[6];
	void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geoCode, bool useGeoCode = false);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
public:
	Shader();
	
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation, const char* geoLocation = "", bool useGeoCode = false);
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
	GLuint GetOmniLightPos()const;
	GLuint GetFarPlane()const;
	
	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLight(PointLight* pLight, unsigned int pCount);
	void SetSpotLights(SpotLight* sLight, unsigned int sCount);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4 *lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);
	void UseShader();
	void ClearShader();
	~Shader();
};

