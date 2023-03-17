#pragma once
#include "Light.h"
class PointLight : public Light
{
protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;
	//we need the equation 1/(ax^2+bx+c) where a = constant, b equals linear, and c equals constant because of how light actually drops off
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, 
				GLfloat xPos,GLfloat yPos, GLfloat zPos, GLfloat con, GLfloat lin, GLfloat exp);
	void UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation, 
		GLuint positionLocation, GLuint constantLocation,GLuint linearLocation, GLuint exponentLocation);
	~PointLight();
};

