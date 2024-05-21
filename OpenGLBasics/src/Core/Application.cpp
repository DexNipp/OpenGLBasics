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


        m_Camera = std::make_unique<Camera>(glm::radians(100.0f), m_Window->GetWidth() / m_Window->GetHeight(), 0.1f, 1000.0f);
        m_CameraController = std::make_unique<CameraController>(*m_Camera);

        m_Camera->Position = glm::vec3(0.0f, 0.0f, 10.0f);
        m_Camera->Fov = 80.0f;

        glEnable(GL_DEPTH_TEST);
	}


    static glm::vec3 lightPos(0.5);
    static glm::vec2 lightOrbit(2.0f);
    static glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	void Application::Run() {

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

        struct Material {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;
        };

        static unsigned int polyMode = GL_FILL;

        glm::mat4 model(1.0f);
        glm::vec4 color(1.0f, 0.55f, 0.60f, 1.00f);

        static float s_RotationSpeed = 0.01f;

        static Material shadingMaterial;
        shadingMaterial.ambient = { 0.3f, 0.3f, 0.3f };
        shadingMaterial.diffuse = { 0.5f, 0.5f, 0.5f };
        shadingMaterial.specular = { 1.0f, 1.0f, 1.0f };
        shadingMaterial.shininess = 32;

		while (m_Running) {

            float time = Time::GetTime();
            m_Timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_Renderer->Clear();
            

            // set light position and orbit
            lightPos[0] = lightOrbit[0] * sin(time);
            lightPos[2] = lightOrbit[1] * cos(time);


            glm::mat4 view = m_Camera->getViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(m_Camera->Fov), m_Camera->AspectRatio, 0.1f, 1000.0f);

            model = glm::rotate(model, -s_RotationSpeed, glm::vec3(0.1f, 1.0f, 0.0f));

            CubeShader.Bind();
            CubeShader.SetUniformMat4("model", model);
            CubeShader.SetUniformMat4("projection", projection);
            CubeShader.SetUniformMat4("view", view);

            CubeShader.SetUniform3f("objectColor", color[0], color[1], color[2]);
            CubeShader.SetUniform3f("lightColor", lightColor);
            CubeShader.SetUniform3f("lightPos", lightPos);
            CubeShader.SetUniform3f("viewPos", m_Camera->Position);

            CubeShader.SetUniform3f("material.ambient", shadingMaterial.ambient);
            CubeShader.SetUniform3f("material.diffuse", shadingMaterial.diffuse);
            CubeShader.SetUniform3f("material.specular", shadingMaterial.specular);
            CubeShader.SetUniform1f("material.shininess", shadingMaterial.shininess);
            //renderer.DrawElements(vaCube, ib, CubeShader);
            m_Renderer->DrawArrays(vaCube, CubeShader);

            LightShader.Bind();
            LightShader.SetUniformMat4("projection", projection);
            LightShader.SetUniformMat4("view", view);

            glm::mat4 lightmodel(1.0f);

            lightmodel = glm::translate(lightmodel, lightPos);
            lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
            LightShader.SetUniformMat4("model", lightmodel);
            LightShader.SetUniform3f("lightColor", lightColor);

            m_Renderer->DrawArrays(vaLightCube, LightShader);


            m_Window->OnUpdate();
		}
	}

    void Application::OnEvent(Event& e) {
        GLB_INFO("Event: {0}", e.ToString());
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

    bool Application::OnKeyboardInput(KeyPressedEvent& e) {
        m_CameraController->ProcessKeyboardInput(e.GetKeyCode(), m_Timestep.GetSeconds());
        return true;
    }
}