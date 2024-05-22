#pragma once 

#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>


namespace glb {

	class ImGuiHelper {
	public:
		void Initialize(GLFWwindow* window);
		void Terminate();

		void BeginFrame();
		void EndFrame();
	};
}