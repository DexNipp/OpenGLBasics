#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

#include <utility>

namespace glb {

	class Input	{
	public:
		inline static bool IsKeyPressed(KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseCodePressed(MouseCode mousecode) { return s_Instance->IsMouseButtonPressedImpl(mousecode); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }

	private:
		bool IsKeyPressedImpl(KeyCode keycode);
		bool IsMouseButtonPressedImpl(MouseCode mousecode);
		std::pair<float, float> GetMousePosImpl();

	private:
		static Input* s_Instance;
	};
}