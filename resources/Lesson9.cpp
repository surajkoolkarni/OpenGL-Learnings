#include "Shader.h"
#include "GLWindow.h"
#include "VertexArray.h"
#include "BufferFactory.h"
#include "VertexAttributes.h"
#include "Texture2D.h"
#include "StbImage.h"
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
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

// vector<unsigned int> indices = {  
//     0, 1, 3, // first triangle
//     1, 2, 3  // second triangle
// };

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void processMouseInput(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 400.f, lastY = 300.f, yaw = -90.f, pitch = 0.f;
    static bool firstMouse = true;

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

int main()
{
    GLWindow::GetInstance().CreateWindow(800, 600, "LearnOpenGL");

    // load opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // create shader objects
    // vertex and fragment shaders
    Shader shader("Lesson9.vs", "Lesson9.fs");

    VertexArray VAO;
    VAO.Bind();

    // create vertex buffer object
    auto vertexBuffer = BufferFactory::CreateBuffer("Vertex");
    vertexBuffer->Copy(vertices.data(), vertices.size());

    // auto elementBuffer = BufferFactory::CreateBuffer("Element");
    // elementBuffer->Copy(indices.data(), indices.size());

    // vertices
    VertexAttributes::SetVertexAttributes(0, 3, 5, 0);
    VertexAttributes::SetVertexAttributes(1, 2, 5, 3);

    cout << "Specified vertex attributes\n";

    StbImage image("container.jpg");
    cout << "Load image\n";

    Texture2D tex;
    tex.Bind();

    tex.SetParameterInt(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex.SetParameterInt(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex.SetParameterInt(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    tex.SetParameterInt(GL_TEXTURE_MAG_FILTER, GL_LINEAR);    

    tex.BindImage(image.width, image.height, image.data);
    
    cout << "Bind texture\n";

    image.Free();
    cout << "Freed image\n";

    shader.use();
    shader.setInt("ourTexture", 0);

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

    GLWindow::GetInstance().SetMouseCB(processMouseInput);
    GLWindow::GetInstance().SetScrollCB(processScrollInput);
    // render loop
    // -----------
    float lastFrameTime = 0.f;
    while (!GLWindow::GetInstance().ShouldWindowClose())
    {
        float currentFrameTime = glfwGetTime();
        camera.MovementSpeed = 2.5f * (currentFrameTime - lastFrameTime);

        GLWindow::GetInstance().ProcessKeyboardInput(processKeyboardInput);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        glEnable(GL_DEPTH_TEST);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        // camera
        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        shader.use();

        VAO.Bind();

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            float time = (float)glfwGetTime();
            model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        tex.Bind();

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        GLWindow::GetInstance().SwapBuffers();

        lastFrameTime = currentFrameTime;
    }

    vertexBuffer->Delete();
    // elementBuffer->Delete();

    tex.Delete();

    VAO.Delete();

    // terminate
    GLWindow::GetInstance().Terminate();

    return 0;
}
