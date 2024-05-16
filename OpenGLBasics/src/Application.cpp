#include "Core/Renderer.h"
#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/IndexBuffer.h"
#include "Core/VertexArray.h"
#include "Core/Shader.h"
#include "Core/Timestep.h"
#include "Camera.h"
#include "CameraController.h"

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

     glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    //////////////////////////////////////////////////////////////////////
    {
        float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };


        float cubeVertices[] = {
            // Front Face
            -0.5,  0.5, -0.5,  0.0, 0.0, 1.0,
            -0.5, -0.5, -0.5,  0.0, 0.0, 1.0,
             0.5, -0.5, -0.5,  0.0, 0.0, 1.0,
             0.5,  0.5, -0.5,  0.0, 0.0, 1.0,

             // Back Face
            -0.5,  0.5,  0.5,  0.0, 0.0, -1.0,
            -0.5, -0.5,  0.5,  0.0, 0.0, -1.0,
             0.5, -0.5,  0.5,  0.0, 0.0, -1.0,
             0.5,  0.5,  0.5,  0.0, 0.0, -1.0,
             
             // Top Face
            -0.5,  0.5,  0.5,  0.0, 1.0, 0.0,
            -0.5,  0.5, -0.5,  0.0, 1.0, 0.0,
             0.5,  0.5, -0.5,  0.0, 1.0, 0.0,
             0.5,  0.5,  0.5,  0.0, 1.0, 0.0,

             // Bottom Face
            -0.5, -0.5,  0.5,  0.0, -1.0, 0.0,
            -0.5, -0.5, -0.5,  0.0, -1.0, 0.0,
             0.5, -0.5, -0.5,  0.0, -1.0, 0.0,
             0.5, -0.5,  0.5,  0.0, -1.0, 0.0,

             // Left Face
            -0.5,  0.5,  0.5,  -1.0, 0.0, 0.0,
            -0.5, -0.5,  0.5,  -1.0, 0.0, 0.0,
            -0.5, -0.5, -0.5,  -1.0, 0.0, 0.0,
            -0.5,  0.5, -0.5,  -1.0, 0.0, 0.0,

            // Right Face
            0.5,  0.5,  0.5,  1.0, 0.0, 0.0,
            0.5, -0.5,  0.5,  1.0, 0.0, 0.0,
            0.5, -0.5, -0.5,  1.0, 0.0, 0.0,
            0.5,  0.5, -0.5,  1.0, 0.0, 0.0
        };

        

        unsigned int cubeIndices[] = {
           // Front Face
           0, 1, 2,
           2, 3, 0,

           // Back Face
           4, 5, 6,
           6, 7, 4,

           // Top Face
           8, 9, 10,
           10, 11, 8,

           // Bottom Face
           12, 13, 14,
           14, 15, 12,

           // Left Face
           16, 17, 18,
           18, 19, 16,

           // Right Face
           20, 21, 22,
           22, 23, 20
        };

        VertexArray vaCube;
        VertexBuffer vb(vertices, 6 * 36 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3); // Normal attribute 
        vaCube.AddBuffer(vb, layout);
        IndexBuffer ib(cubeIndices, 36 * sizeof(unsigned int));
        Shader CubeShader("res/shaders/phong.shader");

        VertexArray vaLightCube;
        VertexBuffer vbLight(cubeVertices, 3 * 24 * sizeof(float));
        VertexBufferLayout layoutLight;
        layoutLight.Push<float>(3);
        layoutLight.Push<float>(3);
        vaLightCube.AddBuffer(vbLight, layoutLight);
        Shader LightShader("res/shaders/light.shader");
        
        Renderer renderer;


        // Given to shader
        glm::vec4 color(1.0f, 0.55f, 0.60f, 1.00f);
        
        glm::vec3 lightPos(0.5f, 1.0f, 2.0f);
   
        //////////////////////////////////////////////////////////////////////

        vb.Unbind();
        ib.Unbind();
        vaCube.Unbind();
        CubeShader.Unbind();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
        ImGui::StyleColorsDark();

        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LEQUAL);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CCW);
        
        

        GLenum mode = GL_FILL;
        float LastFrameTime = 0.0;
        glm::mat4 model(1.0f);
        camera.Position = glm::vec3(0.0f, 0.0f, 20.0f);

        while (!glfwWindowShouldClose(window))
        {
            
            float time = (float)glfwGetTime();
            Timestep timestep = time - LastFrameTime;
            LastFrameTime = time;

            renderer.Clear();
            
            cameraController.ProcessKeyboardInput(window, timestep.GetSeconds());

            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), camera.AspectRatio, 0.1f, 1000.0f);
            
            model = glm::rotate(model, timestep.GetSeconds(), glm::vec3(0.0f, 1.0f, 0.0f));
            
            
            CubeShader.Bind();
            CubeShader.SetUniformMat4("model", model);
            CubeShader.SetUniformMat4("projection", projection);
            CubeShader.SetUniformMat4("view", view);
            CubeShader.SetUniform3f("objectColor", color[0], color[1], color[2]);
            CubeShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
            CubeShader.SetUniform3f("lightPos", lightPos[0], lightPos[1], lightPos[2]);
            CubeShader.SetUniform3f("viewPos", camera.Position[0], camera.Position[1], camera.Position[2]);

            //renderer.DrawElements(vaCube, ib, CubeShader);
            renderer.DrawArrays(vaCube, CubeShader);

           LightShader.Bind();
           LightShader.SetUniformMat4("projection", projection);
           LightShader.SetUniformMat4("view", view);
           
           glm::mat4 lightmodel = glm::mat4(1.0f);
           lightmodel = glm::translate(lightmodel, lightPos);
           lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
           LightShader.SetUniformMat4("model", lightmodel);
           
           renderer.DrawElements(vaLightCube, ib, LightShader);

            //std::cout << "Position: " << camera.Position[0] << camera.Position[1] << camera.Position[2] << std::endl;
            //std::cout << "Front: " << camera.Front[0] << camera.Front[1] << camera.Front[2] << std::endl;
            //std::cout << "Up: " << camera.Up[0] << camera.Up[1] << camera.Up[2] << std::endl;   

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {

                ImGui::Begin("Test");
                ImGui::Text(" ");
                ImGui::ColorEdit3("obj color", (float*)&color);
                //ImGui::SliderFloat("Rotation Speed", &s_RotationSpeed, 0.0f, 0.15f);
                if (ImGui::Button("Change Rotation Axis")) {

                }
                if (ImGui::Button("Toggle WireFrame")) {

                    mode = (mode == GL_FILL) ? GL_LINE : GL_FILL;
                    glPolygonMode(GL_FRONT_AND_BACK, mode);

                }

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

                ImGui::End();
            }

            if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


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
    cameraController.ProcessMouseMovement(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraController.ProcessMouseScroll(yoffset);
}