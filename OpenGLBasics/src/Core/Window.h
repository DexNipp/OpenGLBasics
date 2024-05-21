#pragma once

#include "OpenGLContext.h"
#include "Events/Event.h"
#include <string>
#include <memory>

namespace glb {

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "OpenGL Basics",
			unsigned int width = 1920,
			unsigned int height = 1080)
			:Title(title), Width(width), Height(height)
		{
		}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		~Window();

		void OnUpdate();

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());

		GLFWwindow* GetNativeWindow() { return m_Window; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		OpenGLContext* m_Context;

	private:
		GLFWwindow* m_Window;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}