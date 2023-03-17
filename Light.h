#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"

#include "ShadowMap.h"
class Light
{
protected:
	glm::vec3 color;
	GLfloat  ambiantIntensity;
	GLfloat diffuseIntensity;
	
	ShadowMap* shadowMap;
	glm::mat4 lightProj;
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green,GLfloat blue, GLfloat aIntensity,GLfloat dIntensity);
	ShadowMap* GetShadowMap() const{ return shadowMap; }
	virtual void UseLight(GLuint  ambiantIntensityLocation, GLuint  ambiantColorLocation, GLuint  diffuseIntensityLocation);
	~Light();
};

