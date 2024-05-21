#pragma once

#ifdef ASSERTIONS_ENABLED	

	#define ASSERT(x) if (!(x)) __debugbreak()

	#define GLB_ASSERT(x, ...) { if(!(x)) { GLB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#endif // GLB_ASSERTS

