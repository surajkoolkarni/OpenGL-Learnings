#include "Shader.h"
#include "GLWindow.h"
#include "VertexArray.h"
#include "BufferFactory.h"
#include "Texture2D.h"
#include "Camera.h"
#include "StbImage.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;


// vertices of trisngle
vector<float> vertices = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

vector<glm::vec3> cubePositions = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

Camera camera(glm::vec3(1.16f, 1.44f, 4.41f));

float lastX = 960.f;
float lastY = 540.f;

float yaw = -90.f, pitch = 0.f;
bool firstMouse = true;

void processMouseInput(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processScrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}

auto processKeyboardInput = [](GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Position += camera.MovementSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Position -= camera.MovementSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.MovementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.MovementSpeed;
};

int main()
{
    const int width = 1920;
    const int height = 1080;

    GLWindow::GetInstance().CreateWindow(width, height, "LearnOpenGL");

    // load opengl function pointers
    if (!GLWindow::GetInstance().LoadGLLoader())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Texture2D tex("container2.png");
    tex.Bind();
    tex.SetParameterInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex.SetParameterInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex.SetParameterInt(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    tex.SetParameterInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto vertexBuffer = BufferFactory::CreateBuffer("Vertex");
    vertexBuffer->Copy(vertices.data(), vertices.size());
    vertexBuffer->Bind();

    VertexArray VAO;
    VAO.Bind();
    VAO.SetVertexAttributes(0, 3, 8, 0);
    VAO.SetVertexAttributes(1, 3, 8, 3);
    VAO.SetVertexAttributes(2, 2, 8, 6);

    VertexArray lightVAO;
    lightVAO.Bind();
    lightVAO.SetVertexAttributes(0, 3, 8, 0);

    Shader shader("Lesson14SmoothEdge.vs", "Lesson14SmoothEdge.fs");
    // Shader shader("Lesson14SpotLight.vs", "Lesson14SpotLight.fs");

    shader.use();
    shader.setInt("material.diffuse", 0);

    Shader lightingShader("lighting14.vs", "lighting14.fs");

    GLWindow::GetInstance().SetMouseCB(processMouseInput);
    GLWindow::GetInstance().SetScrollCB(processScrollInput);

    glm::vec3 lightColor(1.f, 1.f, 1.f);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    float radius = 4.f;

    while (!GLWindow::GetInstance().ShouldWindowClose())
    {
        GLWindow::GetInstance().ProcessKeyboardInput(processKeyboardInput);
        
        GLWindow::GetInstance().SetBackgroundColor(0.1f, 0.1f, 0.1f, 1.0f);
        GLWindow::GetInstance().EnableDepthTest();

        // camera
        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), float(width)/ float(height), 0.1f, 100.0f);

        glm::mat4 model(1.f);

        lightPos.x = sin(glfwGetTime()) * sin(glfwGetTime() + glm::radians(90.f)) * radius;
        lightPos.z = cos(glfwGetTime()) * sin(glfwGetTime() + glm::radians(90.f)) * radius;
        lightPos.y = cos(glfwGetTime() + glm::radians(90.f)) * radius;

        shader.use();
        shader.setVec3("viewPos", camera.Position);

        shader.setVec3("material.specular", .5f, .5f, .5f);
        shader.setFloat("material.shininess", 64.f);

        shader.setVec3("light.ambient", 0.2, 0.2, 0.2);
        shader.setVec3("light.diffuse", 0.5, 0.5, 0.5);
        shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        
        shader.setVec3("light.direction", camera.Front);
        shader.setVec3("light.position", camera.Position);
        shader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));

        shader.setFloat("light.constant", 1.0f);
        shader.setFloat("light.linear", 0.09f);
        shader.setFloat("light.quadratic", 0.032f);

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            VAO.Bind();
            VAO.DrawVertexArrays(36);
        }

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(.2f));

        lightingShader.use();
        lightingShader.setVec3("lightColor", lightColor);

        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        lightVAO.Bind();
        lightVAO.DrawVertexArrays(36);

        tex.Activate();
        tex.Bind();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        GLWindow::GetInstance().SwapBuffers();
    }

    vertexBuffer->Delete();

    tex.Delete();

    VAO.Delete();
    lightVAO.Delete();

    // terminate
    GLWindow::GetInstance().Terminate();

    return 0;
}
