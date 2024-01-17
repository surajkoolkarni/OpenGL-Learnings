#include "Shader.h"
#include "GLWindow.h"
#include "VertexArray.h"
#include "BufferFactory.h"
#include "VertexAttributes.h"
#include "Texture2D.h"
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
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};

vector<unsigned int> indices = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

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
    Shader shader("Lesson7.vs", "Lesson7.fs");

    VertexArray::GetInstance().Bind();

    // create vertex buffer object
    auto vertexBuffer = BufferFactory::CreateBuffer("Vertex");
    vertexBuffer->Copy(vertices.data(), vertices.size());

    auto elementBuffer = BufferFactory::CreateBuffer("Element");
    elementBuffer->Copy(indices.data(), indices.size());

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

    // render loop
    // -----------
    while (!GLWindow::GetInstance().ShouldWindowClose())
    {
        // input
        // -----
        GLWindow::GetInstance().ProcessInput();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        // shader.set3F("ourColor", 0.0f, greenValue, 0.0f);

        glm::mat4 transform(1.f);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.f, 0.f, 1.f));
        transform = glm::scale(transform, glm::vec3(2.0f, 2.0f, 2.0f));

        // convert local to world coordinates
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shader.use();

        shader.setMat4("transform", transform);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        VertexArray::GetInstance().Bind();

        tex.Bind();

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        GLWindow::GetInstance().SwapBuffers();
    }

    vertexBuffer->Delete();
    elementBuffer->Delete();

    tex.Delete();

    VertexArray::GetInstance().Delete();

    // terminate
    GLWindow::GetInstance().Terminate();

    return 0;
}
