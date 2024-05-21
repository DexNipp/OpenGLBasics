#include "OpenGLContext.h"
#include "Log.h"
#include "Asserts.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

namespace glb {

	OpenGLContext::OpenGLContext(GLFWwindow* contextWindow)
		: m_ContextWindow(contextWindow) 
	{
		GLB_ASSERT(contextWindow, "contextWindow is null");
	}
	
	void OpenGLContext::Init() {

		glfwMakeContextCurrent(m_ContextWindow);

		
		GLB_ASSERT(glewInit() == GLEW_OK, "Failed to initiate glew");
		
		GLB_INFO("OpenGL Info:");
		GLB_INFO("  Vendor: {0}", (char *)glGetString(GL_VENDOR));
		GLB_INFO("  Renderer: {0}", (char *)glGetString(GL_RENDERER));
		GLB_INFO("  Version: {0}", (char *)glGetString(GL_VERSION));

	}
	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_ContextWindow);
	}
}