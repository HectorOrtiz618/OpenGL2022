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

PointLight::PointLight(GLfloat shadowWidth,GLfloat shadowHeight, GLfloat nearP, GLfloat farP, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
						GLfloat dIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth,shadowHeight,red,green,blue,aIntensity,dIntensity)
{
	position.x = xPos;
	position.y = yPos;
	position.z = zPos;
	
	constant = con;
	linear = lin;
	exponent = exp;

	nearPlane = nearP;
	farPlane = farP;

	float aspect = (float)shadowWidth / (float)shadowHeight;
	lightProj = glm::perspective(glm::radians(90.0f),aspect,nearP,farP);

	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
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

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	//x, -x
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	
	//y, -y
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	//z,-z
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}

glm::vec3 PointLight::GetPosition() const
{
	return position;
}


GLfloat PointLight::GetFarPlane() const
{
	return farPlane;
}

PointLight::~PointLight()
{
}
