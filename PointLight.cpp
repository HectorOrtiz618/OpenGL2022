#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;

}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, 
						GLfloat dIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(1024,1024,red,green,blue,aIntensity,dIntensity)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation, 
							GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{

	glUniform3f(ambiantColorLocation, color.x, color.y, color.z);
	glUniform1f(ambiantIntensityLocation, ambiantIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight()
{
}
