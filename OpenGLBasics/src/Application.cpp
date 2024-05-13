#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Core/Renderer.h"

#include "Core/VertexBuffer.h"
#include "Core/VertexBufferLayout.h"
#include "Core/IndexBuffer.h"
#include "Core/VertexArray.h"
#include "Core/Shader.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1080, 1080, "OpenGLBasics", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

     glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    //////////////////////////////////////////////////////////////////////
    {
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.8f,
            -0.5f,  0.8f
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        float cubeVertices[] = {
            -1.0, -1.0,  1.0,
             1.0, -1.0,  1.0,
            -1.0,  1.0,  1.0,
             1.0,  1.0,  1.0,
            -1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
            -1.0,  1.0, -1.0,
             1.0,  1.0, -1.0,
        };

        unsigned int cubeIndices[] = {
           0, 2, 3, 0, 3, 1,
           2, 6, 7, 2, 7, 3,
           6, 4, 5, 6, 5, 7,
           4, 0, 1, 4, 1, 5,
           0, 4, 6, 0, 6, 2,
           1, 5, 7, 1, 7, 3,
        };

        VertexArray va;
        VertexBuffer vb(cubeVertices, 3 * 8 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);

        va.AddBuffer(vb, layout);

        IndexBuffer ib(cubeIndices, sizeof(cubeIndices));

        Shader shader("res/shaders/3d.shader");
       
        Renderer renderer;

        vb.Unbind();
        ib.Unbind();
        va.Unbind();
        shader.Unbind();
        

        // Given to shader
        glm::vec4 color(0.45f, 0.55f, 0.60f, 1.00f);
        
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(65.0f), 1080.0f / 1080.0f, 0.1f, 100.0f);

        //////////////////////////////////////////////////////////////////////

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
        ImGui::StyleColorsDark();
        
        static float s_RotationSpeed;

        glEnable(GL_DEPTH_TEST);

       // Main loop
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            shader.Bind();
            

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                
                ImGui::Begin("Test");                       
                ImGui::Text(" ");
                ImGui::ColorEdit3("obj color", (float*)&color);
                ImGui::SliderFloat("Rotation Speed", &s_RotationSpeed, 0.0f, 0.15f);
                if (ImGui::Button("Change Rotation Axis")) {
                   
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                }
             
                //ImGui::Text("counter = %d", counter);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            
                ImGui::End();
            }
           
            model = glm::rotate(model, s_RotationSpeed * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            shader.SetUniformMat4("model", model);
            shader.SetUniformMat4("view", view);
            shader.SetUniformMat4("projection", projection);
            shader.SetUniform4f("u_Color", color[0], color[1], color[2], 1.0f);
            
            renderer.Draw(va, ib, shader);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
        
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}