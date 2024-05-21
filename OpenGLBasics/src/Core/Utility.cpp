#include "Utility.h"

#include <GLFW/glfw3.h>

namespace glb {

	float glb::Time::GetTime()
	{
		return (float)glfwGetTime();
	}
}