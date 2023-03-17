#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambiantIntensity = 1.f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue);
	ambiantIntensity = aIntensity;

	diffuseIntensity = dIntensity;
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

void Light::UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation)
{
	glUniform3f(ambiantColorLocation, color.x, color.y, color.z);
	glUniform1f(ambiantIntensityLocation,ambiantIntensity);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
{
}
