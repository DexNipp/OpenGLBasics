#pragma once

struct GLFWwindow;

namespace glb {

	class OpenGLContext {
	public:
		OpenGLContext(GLFWwindow* contextWindow);

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_ContextWindow;
	};
}
