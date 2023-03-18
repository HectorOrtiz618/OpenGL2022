#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambiantIntensity = 1.f;
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 1.0f;
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity):Light(shadowWidth,shadowHeight,red,green,blue,aIntensity,dIntensity)
{

	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-20.0f, 20.0f, -50.0f, 50.0f, 0.1f, 200.f);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void DirectionalLight::UseLight(GLuint ambiantIntensityLocation, GLuint ambiantColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambiantColorLocation, color.x, color.y, color.z);
	glUniform1f(ambiantIntensityLocation, ambiantIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
	//delete shadowMap;
	//shadowMap = nullptr;
}
