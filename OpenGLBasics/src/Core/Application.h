#pragma once
#include "Window.h"
#include "glb.h"

#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Events/InputEvents.h"

#include "Renderer/CameraController.h"
#include "Renderer/Camera.h"

#include <memory>
#include <utility> 

namespace glb {

	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	class Application {
	public:
		Application();
		~Application() = default;

		void Run();

		void OnEvent(Event& e);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

		static Application* CreateApplication();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<Camera> m_Camera;
		std::unique_ptr<CameraController> m_CameraController;

	private:
		bool m_Running = true;

		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};
}