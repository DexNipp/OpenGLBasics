#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Shapes.h"
#include "Timestep.h"
#include "Camera.h"
#include "CameraController.h"
#include "Log.h"

#include "GLFW/glfw3.h"
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

const float WIDTH = 1920.0f;
const float HEIGHT = 1080.0f;

Camera camera(glm::radians(100.0f), WIDTH / HEIGHT, 0.1f, 1000.0f);
CameraController cameraController(camera);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLBasics", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, inputCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        GLB_ERROR("glew failed to init");

    std::cout << glGetString(GL_VERSION) << std::endl;
    

    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
        ImGui::StyleColorsDark();

        //////////////////////////////////////////////////////////////////////

        Shapes::Cube Cube;

        VertexArray vaCube;
        VertexBuffer vb(Cube.vertices, Cube.arraySize * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3); // Normal attribute 
        vaCube.AddBuffer(vb, layout);
        //IndexBuffer ib(cubeIndices, 36 * sizeof(unsigned int));
        Shader CubeShader("res/shaders/phong.shader");

        VertexArray vaLightCube;
        VertexBuffer vbLight(Cube.vertices, Cube.arraySize * sizeof(float));
        VertexBufferLayout layoutLight;
        layoutLight.Push<float>(3);
        layoutLight.Push<float>(3);
        vaLightCube.AddBuffer(vbLight, layoutLight);
        Shader LightShader("res/shaders/light.shader");

        Renderer renderer;

        //////////////////////////////////////////////////////////////////////

        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LEQUAL);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CCW);

        struct Material {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;
        };
        
        float LastFrameTime = 0.0;
        static unsigned int polyMode = GL_FILL;

        glm::mat4 model(1.0f);
        glm::vec4 color(1.0f, 0.55f, 0.60f, 1.00f);

        camera.Position = glm::vec3(0.0f, 0.0f, 10.0f);
        camera.Fov = 80.0f;

        static float s_RotationSpeed = 0.01f;

        static glm::vec3 lightPos(0.5);
        static glm::vec2 lightOrbit(2.0f);
        static glm::vec3 lightColor(1.0f, 1.0f, 1.0f);


        static Material shadingMaterial;
        shadingMaterial.ambient = { 1.0f, 0.5f, 0.31f };
        shadingMaterial.diffuse = { 1.0f, 0.5f, 0.31f };
        shadingMaterial.specular = { 0.5f, 0.5f, 0.5f };
        shadingMaterial.shininess = 32;

        while (!glfwWindowShouldClose(window))
        {      
            float time = (float)glfwGetTime();
            Timestep timestep = time - LastFrameTime;
            LastFrameTime = time;

            renderer.Clear();
            cameraController.ProcessKeyboardInput(window, timestep.GetSeconds());

            // set light position and orbit
            lightPos[0] = lightOrbit[0] * sin(time);
            lightPos[2] =  lightOrbit[1] * cos(time);
            

            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), camera.AspectRatio, 0.1f, 1000.0f);
            
            model = glm::rotate(model, -s_RotationSpeed, glm::vec3(0.1f, 1.0f, 0.0f));
            
            CubeShader.Bind();
            CubeShader.SetUniformMat4("model", model);
            CubeShader.SetUniformMat4("projection", projection);
            CubeShader.SetUniformMat4("view", view);

            CubeShader.SetUniform3f("objectColor", color[0], color[1], color[2]);
            CubeShader.SetUniform3f("lightColor", lightColor);
            CubeShader.SetUniform3f("lightPos",lightPos);
            CubeShader.SetUniform3f("viewPos", camera.Position);

            CubeShader.SetUniform3f("material.ambient", shadingMaterial.ambient);
            CubeShader.SetUniform3f("material.diffuse", shadingMaterial.diffuse);
            CubeShader.SetUniform3f("material.specular", shadingMaterial.specular);
            CubeShader.SetUniform1f("material.shininess", shadingMaterial.shininess);
             //renderer.DrawElements(vaCube, ib, CubeShader);
            renderer.DrawArrays(vaCube, CubeShader); 

            LightShader.Bind();
            LightShader.SetUniformMat4("projection", projection);
            LightShader.SetUniformMat4("view", view);

            glm::mat4 lightmodel(1.0f);

            lightmodel = glm::translate(lightmodel, lightPos);
            lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
            LightShader.SetUniformMat4("model", lightmodel);
            LightShader.SetUniform3f("lightColor", lightColor);
            
            renderer.DrawArrays(vaLightCube, LightShader);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            
            ImGui::NewFrame();
            {
                ImGui::ShowDemoWindow();

                ImGui::Begin("Debug");

                if (ImGui::CollapsingHeader("Scene Parameters")) {
                    ImGui::ColorEdit3("obj color", (float*)&color);
                    ImGui::SliderFloat("Rotation Speed", &s_RotationSpeed, 0.0f, 0.15f);
                    ImGui::Spacing();

                    ImGui::ColorEdit3("light color", (float*)&lightColor);
                    ImGui::SliderFloat3("Light Position", &lightPos[0], -10.0f, 10.0f);
                    ImGui::SliderFloat2("Light Orbit", &lightOrbit[0], 0.0f, 10.0f);
                }
                ImGui::Spacing();
                if (ImGui::CollapsingHeader("Lighting Parameters")) {
                    ImGui::SliderFloat3("ambient", &shadingMaterial.ambient[0], 0.0f, 1.0f);
                    ImGui::SliderFloat3("diffuse", &shadingMaterial.diffuse[0], 0.0f, 1.0f);
                    ImGui::SliderFloat3("specular", &shadingMaterial.specular[0], 0.0f, 1.0f);
                    ImGui::SliderFloat("shininess", &shadingMaterial.shininess, 0.0f, 100.0f);
                }
                ImGui::Spacing();
                if (ImGui::Button("Toggle WireFrame")) {
                    polyMode = (polyMode == GL_FILL) ? GL_LINE : GL_FILL;
                    glPolygonMode(GL_FRONT_AND_BACK, polyMode);
                }
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
        
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

    unsigned int mouseMode = glfwGetInputMode(window, GLFW_CURSOR);

    if (mouseMode == GLFW_CURSOR_DISABLED)
        cameraController.ProcessMouseMovement(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraController.ProcessMouseScroll(yoffset);
}

void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        unsigned int mouseMode = glfwGetInputMode(window, GLFW_CURSOR);
        mouseMode = (mouseMode == GLFW_CURSOR_NORMAL) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
        glfwSetInputMode(window, GLFW_CURSOR, mouseMode);
    }
}