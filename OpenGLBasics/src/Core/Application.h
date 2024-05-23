#pragma once
#include "glb.h"
#include "Window.h"
#include "ImGuiHelper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <memory>
#include <utility> 

namespace glb {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	class Application {
	public:
		Application();
		~Application() = default;

		void Run();

		void OnImGuiUpdate();
		void OnEvent(Event& e);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		static Application* CreateApplication();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		Ref<Window> m_Window;
		Ref<ImGuiHelper> m_ImGuiHelper;
		Ref<Renderer> m_Renderer;
		Ref<Camera> m_Camera;
		Ref<CameraController> m_CameraController;

	private:
		bool m_Running = true;
		bool m_IsCursorEnabled = true;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

	private: 
		// Scene variables
		Ref<VertexArray> m_CubeVA;
		Ref<VertexArray> m_CubeLightVA;
		Ref<VertexBuffer> m_VB;
		Ref<VertexBufferLayout> m_Layout;
	
		Ref<Shader> m_CubeShader;
		Ref<Shader> m_LightShader;

		struct Material {
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float shininess;
		};

		Ref<Material> m_ShadingMaterial;

		Shapes::Cube m_Cube;

		glm::mat4 m_CubeModel{ 1.0f };
		glm::vec4 m_CubeModelColor{ 1.0f, 0.55f, 0.60f, 1.00f };
		
		glm::vec3 m_LightPos{ 0.5f };
		glm::vec2 m_LightOrbit{ 2.0f };
		glm::vec3 m_LightColor{ 1.0f, 1.0f, 1.0f };

		float m_CubeRotationSpeed = 0.01f;
		unsigned int m_PolygonMode = GL_FILL;

	private:
		static Application* s_Instance;
	};
}