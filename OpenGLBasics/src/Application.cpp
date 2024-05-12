#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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


        VertexArray va;
        VertexBuffer vb(positions, 2 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, sizeof(indices));

        Shader shader("res/shaders/3d.shader");
       
        Renderer renderer;

        // Given to shader
        glm::vec4 color(0.45f, 0.55f, 0.60f, 1.00f);
        glm::mat4 transform(1.0);

        //transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //////////////////////////////////////////////////////////////////////

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
        ImGui::StyleColorsDark();
        
        static float s_RotationSpeed;
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
                if (ImGui::Button("Rotate 90 degrees")) {
                   
                    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                }
             
                //ImGui::Text("counter = %d", counter);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            
                ImGui::End();
            }
           
            transform = glm::rotate(transform, s_RotationSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
            shader.UploadUniformMat4("transform", transform);
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