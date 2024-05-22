#include "Application.h"
#include "glb.h"

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace glb {

    Application* Application::s_Instance = nullptr;

    Application* Application::CreateApplication() {
        return new Application();
    }

    Application::Application() {

        GLB_ASSERT(!s_Instance, "Application already exists.");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiHelper->Initialize(m_Window->GetNativeWindow());

        m_Camera = std::make_unique<Camera>(glm::radians(100.0f), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 1000.0f);
        m_CameraController = std::make_unique<CameraController>(*m_Camera);
        m_Camera->Position = glm::vec3(0.0f, 0.0f, 10.0f);
        m_Camera->Fov = 80.0f;

        m_Renderer->EnableDepthTest();



        m_CubeVA.reset(VertexArray::Create());
        m_CubeLightVA.reset(VertexArray::Create());

        m_VB.reset(VertexBuffer::Create(m_Cube.vertices, m_Cube.arraySize * sizeof(float)));
        m_Layout.reset(new VertexBufferLayout());
        m_Layout->Push<float>(3); //verts
        m_Layout->Push<float>(3); //normals
        m_CubeVA->AddBuffer(*m_VB, *m_Layout);
        m_CubeLightVA->AddBuffer(*m_VB, *m_Layout);

        m_CubeShader.reset(new Shader("res/shaders/phong.shader"));
        m_LightShader.reset(new Shader("res/shaders/light.shader"));

        m_ShadingMaterial.reset(new Material());
        m_ShadingMaterial->ambient = { 0.3f, 0.3f, 0.3f };
        m_ShadingMaterial->diffuse = { 0.5f, 0.5f, 0.5f };
        m_ShadingMaterial->specular = { 1.0f, 1.0f, 1.0f };
        m_ShadingMaterial->shininess = 32;
    }

	void Application::Run() {

        while (m_Running) {

            float time = Time::GetTime();
            m_Timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_Renderer->Clear();

            m_CameraController->ProcessKeyboardInput(m_Timestep.GetSeconds());

            // set light position and orbit
            m_LightPos[0] = m_LightOrbit[0] * sin(time);
            m_LightPos[2] = m_LightOrbit[1] * cos(time);

            glm::mat4 view = m_Camera->getViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Fov), m_Camera->AspectRatio, 0.1f, 1000.0f);

            m_CubeModel = glm::rotate(m_CubeModel, -m_CubeRotationSpeed, glm::vec3(0.1f, 1.0f, 0.0f));

            m_CubeShader->Bind();
            m_CubeShader->SetUniformMat4("model", m_CubeModel);
            m_CubeShader->SetUniformMat4("projection", projection);
            m_CubeShader->SetUniformMat4("view", view);
                        
            m_CubeShader->SetUniform3f("objectColor", m_CubeModelColor[0], m_CubeModelColor[1], m_CubeModelColor[2]);
            m_CubeShader->SetUniform3f("lightColor", m_LightColor);
            m_CubeShader->SetUniform3f("lightPos", m_LightPos);
            m_CubeShader->SetUniform3f("viewPos", m_Camera->Position);
                        
            m_CubeShader->SetUniform3f("material.ambient", m_ShadingMaterial->ambient);
            m_CubeShader->SetUniform3f("material.diffuse", m_ShadingMaterial->diffuse);
            m_CubeShader->SetUniform3f("material.specular", m_ShadingMaterial->specular);
            m_CubeShader->SetUniform1f("material.shininess", m_ShadingMaterial->shininess);
            //renderer.DrawElements(vaCube, ib, CubeShader);
            m_Renderer->DrawArrays(*m_CubeVA, *m_CubeShader);

            m_LightShader->Bind();
            m_LightShader->SetUniformMat4("projection", projection);
            m_LightShader->SetUniformMat4("view", view);

            glm::mat4 lightmodel(1.0f);

            lightmodel = glm::translate(lightmodel, m_LightPos);
            lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
            m_LightShader->SetUniformMat4("model", lightmodel);
            m_LightShader->SetUniform3f("lightColor", m_LightColor);

            m_Renderer->DrawArrays(*m_CubeLightVA, *m_LightShader);

            m_ImGuiHelper->BeginFrame();
            OnImGuiUpdate();
            m_ImGuiHelper->EndFrame();

            m_Window->OnUpdate();
        }
    }

    void Application::OnImGuiUpdate() {

        ImGui::NewFrame();
        {
            ImGui::Begin("Debug");

            if (ImGui::CollapsingHeader("Scene Parameters")) {
                ImGui::ColorEdit3("obj color", (float*)&m_CubeModelColor);
                ImGui::SliderFloat("Rotation Speed", &m_CubeRotationSpeed, 0.0f, 0.15f);
                ImGui::Spacing();

                ImGui::ColorEdit3("light color", (float*)&m_LightColor);
                ImGui::SliderFloat3("Light Position", &m_LightPos[0], -10.0f, 10.0f);
                ImGui::SliderFloat2("Light Orbit", &m_LightOrbit[0], 0.0f, 10.0f);
            }
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("Lighting Parameters")) {
                ImGui::SliderFloat3("ambient", &m_ShadingMaterial->ambient[0], 0.0f, 1.0f);
                ImGui::SliderFloat3("diffuse", &m_ShadingMaterial->diffuse[0], 0.0f, 1.0f);
                ImGui::SliderFloat3("specular", &m_ShadingMaterial->specular[0], 0.0f, 1.0f);
                ImGui::SliderFloat("shininess", &m_ShadingMaterial->shininess, 0.0f, 100.0f);
            }
            ImGui::Spacing();
            if (ImGui::Button("Toggle WireFrame")) {
                m_PolygonMode = (m_PolygonMode == GL_FILL) ? GL_LINE : GL_FILL;
                glPolygonMode(GL_FRONT_AND_BACK, m_PolygonMode);
            }
            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
    }

    void Application::OnEvent(Event& e) {

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMoved));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Application::OnMouseScrolled));
        //dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyboardInput));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {

        m_Running = false;
        return true;
    }

    bool Application::OnMouseMoved(MouseMovedEvent& e) {

        m_CameraController->ProcessMouseMovement(e.GetX(), e.GetY());
        return true;
    }

    bool Application::OnMouseScrolled(MouseScrolledEvent& e) {

        m_CameraController->ProcessMouseScroll(e.GetOffsetY());
        return true;
    }
}