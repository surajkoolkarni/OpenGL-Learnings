#include "Shader.h"
#include "GLWindow.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_impl_glfw.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};

int main()
{
    const int width = 1920;
    const int height = 1080;

    std::cout << "Setting up camera and window\n";

    // SetupDear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    GLWindow::GetInstance().CreateWindow(width, height, "ModelViewer");

    ImGui_ImplGlfw_InitForOpenGL(GLWindow::GetInstance().Window(), true);

    ImGui_ImplOpenGL3_Init("#version 430");

    // load opengl function pointers
    if (!GLWindow::GetInstance().LoadGLLoader())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "Compiling shaders\n";

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("Lesson16SmoothEdge.vs.glsl", "Lesson16SmoothEdge.fs.glsl");

    std::cout << "Loading model\n";

    std::unique_ptr<Model> model3D = std::make_unique<Model>("backpack.obj");

    std::cout << model3D->BoundingBox()->max.x <<  ", " << model3D->BoundingBox()->max.y << ", " << model3D->BoundingBox()->max.z << std::endl;
    std::cout << model3D->BoundingBox()->min.x <<  ", " << model3D->BoundingBox()->min.y << ", " << model3D->BoundingBox()->min.z << std::endl;

    std::cout << "Main loop\n";

    glm::vec2 translationVec(0.f, 0.f);
    glm::vec3 rotationAxis(0.f, 1.f, 0.f);
    float angle = 0.f;

    model3D->AttachCamera("camera1", std::make_shared<Camera>());

    std::shared_ptr<Camera> camera = model3D->ToggleCamera("camera1");
    
    std::cout << "Position:" << camera->Position.x << ", " << camera->Position.y << ", " << camera->Position.z << std::endl;
    std::cout << "Front:" << camera->Front.x << ", " << camera->Front.y << ", " << camera->Front.z << std::endl;
    std::cout << "Up:" << camera->Up.x << ", " << camera->Up.y << ", " << camera->Up.z << std::endl;
    std::cout << "FocalPoint:" << camera->FocalPoint.x << ", " << camera->FocalPoint.y << ", " << camera->FocalPoint.z << std::endl;

    camera->NearPlane = 194.9658193607427;
    camera->FarPlane = 404.90612273288286;

    std::cout << "NearPlane:" << camera->NearPlane << std::endl;
    std::cout << "FarPlane:" << camera->FarPlane << std::endl;

    while (!GLWindow::GetInstance().ShouldWindowClose())
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Manipulate the model");
        ImGui::SliderFloat2("Pan", &translationVec[0], -5.0f, 5.f); 
        ImGui::SliderFloat("Zoom fov", &camera->Zoom, 0.f, 500.f);
        ImGui::InputFloat3("axis of rotation", &rotationAxis[0]);
        ImGui::SliderAngle("angle of rotation", &angle);

        ImGui::SliderFloat3("Camera Front", &camera->Front[0], -10, 10);
        ImGui::SliderFloat3("Camera Pos", &camera->Position[0], -10, 10);
        ImGui::SliderFloat3("Camera Up", &camera->Up[0], -1, 1);

        ImGui::SliderFloat("Near Plane", &camera->NearPlane, 0.1, 10000);
        ImGui::SliderFloat("Far Plane", &camera->FarPlane, 10, 10000);

        ImGui::End();

        GLWindow::GetInstance().ProcessKeyboardInput();
        
        GLWindow::GetInstance().SetBackgroundColor(0.5f, 0.5f, 0.5f, 1.0f);
        GLWindow::GetInstance().EnableDepthTest();

        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), GLWindow::GetInstance().AspectRatio(), camera->NearPlane, camera->FarPlane);
        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(translationVec, 0.f));
        model = glm::rotate(model, angle, rotationAxis);
        
        shader->use();
        shader->setVec3("viewPos", camera->Position);
        shader->setFloat("shininess", 32.0f);

        shader->setVec3("directionalLight.direction", camera->Front);
        shader->setVec3("directionalLight.ambient", 0.2, 0.2, 0.2);
        shader->setVec3("directionalLight.diffuse", 0.5, 0.5, 0.5);
        shader->setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        
        for (int i = 0; i < 4; ++i)
        {
            shader->setVec3("pointLights[" + to_string(i) + "].position", pointLightPositions[i]);

            shader->setVec3("pointLights[" + to_string(i) + "].ambient", 0.2, 0.2, 0.2);
            shader->setVec3("pointLights[" + to_string(i) + "].diffuse", 0.5, 0.5, 0.5);
            shader->setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);

            shader->setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
            shader->setFloat("pointLights[" + to_string(i) + "].linear", 0.09f);
            shader->setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032f);
        }

        shader->setVec3("spotLight.ambient", 0.2, 0.2, 0.2);
        shader->setVec3("spotLight.diffuse", 0.5, 0.5, 0.5);
        shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        
        shader->setVec3("spotLight.direction", camera->Front);
        shader->setVec3("spotLight.position", camera->Position);
        shader->setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
        shader->setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));

        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        ImGui::Render();

        shader->use();
        model3D->Draw(shader);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLWindow::GetInstance().SwapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout << "Exit main loop\n";

    return 0;
}
