#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}
void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}
void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFromFile(vertexLocation);
    std::string fragmentString = ReadFromFile(fragmentLocation);
    CompileShader(vertexString.c_str(), fragmentString.c_str());
}
std::string Shader::ReadFromFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);
    if (!fileStream.is_open())
    {
        printf("Failed to load %s! File does not exist!", fileLocation);
        return"";
    }
    std::string line = "";


    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line+"\n");
        //line = "";
    }

    fileStream.close();
    return content;
}
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();
    if (!shaderID)
    {
        printf("Compiling Shader Program Failed\n");
        return;
    }
    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Linking Program: %s", &eLog);
    }
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Validating Program: %s", &eLog);
    }
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = (glGetUniformLocation(shaderID, "view"));
    uniformAmbiantColor = glGetUniformLocation(shaderID, "directionalLight.color");
    uniformAmbiantIntensity = glGetUniformLocation(shaderID, "directionalLight.ambiantIntensity");
    uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
    uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

}
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);


    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: %s", shaderType, &eLog);
    }
    glAttachShader(theProgram, theShader);
}
GLuint Shader::GetProjectionLocation() const
{
    return uniformProjection;
}
GLuint Shader::GetModelLocation() const
{
    return uniformModel;
}
GLuint Shader::GetViewLocation() const
{
    return uniformView;
}
GLuint Shader::GetAmbiantIntensity() const
{
    return uniformAmbiantIntensity;
}
GLuint Shader::GetAmbiantColor() const
{
    return uniformAmbiantColor;
}
GLuint Shader::GetDiffuseIntensity() const
{
    return uniformDiffuseIntensity;
}
GLuint Shader::GetDirectionLocation() const
{
    return uniformDirection;
}
GLuint Shader::GetEyePosition() const
{
    return uniformEyePosition;
}
GLuint Shader::GetSpecularIntensity() const
{
    return uniformSpecularIntensity;
}
GLuint Shader::GetShininessLocation() const
{
    return uniformShininess;
}
void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
         glDeleteProgram(shaderID);
         shaderID = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader()
{
    ClearShader();
}
