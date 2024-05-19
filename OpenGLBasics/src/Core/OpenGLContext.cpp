#include "OpenGLContext.h"
#include "Log.h"
#include "Base.h"

#include "GLFW/glfw3.h"
//#include "GL/glew.h"



namespace glb {

	OpenGLContext::OpenGLContext(GLFWwindow* contextWindow)
		: m_ContextWindow(contextWindow) 
	{
		GLB_ASSERT(contextWindow, "contextWindow is null");
	}
	
	void OpenGLContext::Init() {

		glfwMakeContextCurrent(m_ContextWindow);

		//int status = glewInit();
		//GLB_ASSERT(status, "Failed to initiate glew");
		//GLB_INFO("OpenGL Info:");
		//GLB_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//GLB_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//GLB_INFO("  Version: {0}", glGetString(GL_VERSION));

	}
	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_ContextWindow);
	}
}