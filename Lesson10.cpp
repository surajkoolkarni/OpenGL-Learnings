#include "Shader.h"
#include "GLWindow.h"
#include "VertexArray.h"
#include "BufferFactory.h"
#include "Texture2D.h"
#include "Camera.h"

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
    -0.5f, -0.5f, -0.5f, 
    0.5f, -0.5f, -0.5f,  
    0.5f,  0.5f, -0.5f,  
    0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f, -0.5f,  0.5f, 
    0.5f, -0.5f,  0.5f,  
    0.5f,  0.5f,  0.5f,  
    0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f, 

    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 

    0.5f,  0.5f,  0.5f,  
    0.5f,  0.5f, -0.5f,  
    0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f,  0.5f,  
    0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f, 
    0.5f, -0.5f, -0.5f,  
    0.5f, -0.5f,  0.5f,  
    0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f, 

    -0.5f,  0.5f, -0.5f, 
    0.5f,  0.5f, -0.5f,  
    0.5f,  0.5f,  0.5f,  
    0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f, -0.5f, 
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

    auto vertexBuffer = BufferFactory::CreateBuffer("Vertex");
    vertexBuffer->Copy(vertices.data(), vertices.size());
    vertexBuffer->Bind();

    VertexArray VAO;
    VAO.Bind();
    VAO.SetVertexAttributes(0, 3, 3, 0);

    VertexArray lightVAO;
    lightVAO.Bind();
    lightVAO.SetVertexAttributes(0, 3, 3, 0);

    Shader shader("Lesson10.vs", "Lesson10.fs");
    Shader lightingShader("lighting.vs", "lighting.fs");

    GLWindow::GetInstance().SetMouseCB(processMouseInput);
    GLWindow::GetInstance().SetScrollCB(processScrollInput);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
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

        shader.use();
        shader.setVec3("objectColor", 1.f, .5f, .31f);
        shader.setVec3("lightColor", 1.f, 1.f, 1.f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        VAO.Bind();
        VAO.DrawVertexArrays(36);

        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(.2f));

        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.f, .5f, .31f);
        lightingShader.setVec3("lightColor", 1.f, 1.f, 1.f);

        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        lightVAO.Bind();
        lightVAO.DrawVertexArrays(36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        GLWindow::GetInstance().SwapBuffers();
    }

    vertexBuffer->Delete();

    VAO.Delete();
    lightVAO.Delete();

    // terminate
    GLWindow::GetInstance().Terminate();

    return 0;
}
