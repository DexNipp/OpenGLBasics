#include "Window.h"
#include "Base.h"
#include "Log.h"


namespace glb {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GLB_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props) {

		return new Window(props);;
	}

	Window::Window(const WindowProps& props) {
		Init(props);
	}

	Window::~Window() {
		Shutdown();
	}

	void Window::Init(const WindowProps& props) {

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			GLB_ASSERT(success, "Failed to initiate glfw");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);


		// TODO: Set glfwCallbacks here after creating event system
	}
	
	void Window::OnUpdate() {
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void Window::SetVSync(bool enabled) {

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool Window::IsVSync() const {
		return m_Data.VSync;
	}

	void Window::Shutdown() {
		glfwDestroyWindow(m_Window);
	}
}