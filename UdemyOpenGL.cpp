// UdemyOpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "GL/glew.h"
#include "glfw3.h"
#include "GLWindow.h"
#include "Camera.h"
#include "glm/glm.hpp";
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Texture.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"

//Vertex Shader
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
bool bDirection = true;
float triOffset = 0.f;
float triMaxOffset = 0.7f;
float triInfrement = 0.005f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
Texture dirtTexture("textures/dirt.png");  
Texture brickTexture("textures/brick.png");
DirectionalLight mainLight;
PointLight pointLight[MAX_POINT_LIGHTS];
Material shinyMaterial;
Material dullMaterial;
static const char* vShader = "shaders/shader.vert";

static const char* fShader = "shaders/shader.frag";
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }
    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y, vertices[nOffset + 2] = vec.z;
    }
}
void CreateObjects()
{
    unsigned int indices[] =
    {
        0,3,1,
        1,3,2,
        2,3,0,
        0,1,2
    };
    GLfloat vertices[] =
    {
        //x     y     z     u     v         nx  ny  nz
        -1.0f, -1.0f, 0.f,  0.f,  0.f,      0.0f,0.0f,0.0f,
        0.f, -1.0f, 1.0f,   0.5f,  0.f,     0.0f,0.0f,0.0f,
        1.0f,-1.0f, 0.0f,   1.f,   0.f,     0.0f,0.0f,0.0f,
        0.0f, 1.0f, 0.0f,   0.5f,   1.f,    0.0f,0.0f,0.0f
    };
    calcAverageNormals(indices, 12, vertices, 32, 8, 5);
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);
    
    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    //printf("obj1 loaded into meshList\n");
}
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vShader, fShader);
    
    shaderList.push_back(*shader1);
}
int main()
{
    GLWindow mainWindow = GLWindow(1280, 720);
    mainWindow.Initialize();
    
    CreateObjects();
    CreateShaders();
    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f,0.f, 5.0f, 0.5f);
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
  
    brickTexture.LoadTexture();
    dirtTexture.LoadTexture();
    
    shinyMaterial = Material(1.0f,32);
    dullMaterial = Material(0.3f, 4);

    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.2f, 2.0f, -1.0f, -2.0f, 0.3f);
    unsigned int pointLightCount = 0;
    pointLight[0] = PointLight(0.0f, 1.0f, 0.0f, 0.1f, 1.0f, -4.0f, 0.0f, 0.0f, 0.3f, 0.2f, 0.1f);
    pointLightCount++;
    while (!mainWindow.GetShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        glfwPollEvents();
        camera.keyControls(mainWindow.GetKeys(),deltaTime);
        camera.mouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());
        
        //clear window
        glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();

        uniformEyePosition = shaderList[0].GetEyePosition();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensity();
        uniformShininess = shaderList[0].GetShininessLocation();

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLight(pointLight, pointLightCount);
        //mainLight.UseLight(uniformAmbiantIntensity,uniformAmbiantColor,uniformDiffuseIntesity,uniformDirection);
        //glm matrices MUIST be initilized as glm::mat4 model(1.0f); OR YOU WILL GET GARBAGE TRANSFORM DATA
        glm::mat4 model(1.0f);
        
        
        model = glm::translate(model, glm::vec3(0.f,0.f, -2.5f));  
       // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); 

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.GetCameraPosition().x,camera.GetCameraPosition().y,camera.GetCameraPosition().z);
       
        brickTexture.UseTexture();
        shinyMaterial.UseMaterial(uniformSpecularIntensity,uniformShininess);
        meshList[0]->RenderMesh();

        //clearing transforms for new model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, 4.f, -2.5f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();

        glUseProgram(0);

        mainWindow.swapBuffers();

    }
    return 0;
}
