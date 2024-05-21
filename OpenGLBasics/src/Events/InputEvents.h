#pragma once

#include "Event.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

#include "sstream"

namespace glb {

	//////////////////////////////////////////////////////////////////////
	////////////////////////// KeyEvents /////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	class KeyEvent : public Event {
	public:
		inline KeyCode GetKeyCode() { return m_KeyCode; }

	protected:
		KeyEvent(KeyCode keycode)
			: m_KeyCode(keycode)
		{
		}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat)
		{
		}
		
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << ", repeat = " << m_IsRepeat;
			return ss.str();
		}

		EventClassType(KeyPressed)

	private:
		bool m_IsRepeat;

	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EventClassType(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EventClassType(KeyTyped)
	};

	//////////////////////////////////////////////////////////////////////
	////////////////////////// MouseEvents ///////////////////////////////
	//////////////////////////////////////////////////////////////////////

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(const float x, const float y)
			: m_MouseX(x), m_MouseY(y)
		{
		}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
			
		EventClassType(MouseMoved)

	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const float offsetX, const float offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY)
		{
		}

		float GetOffsetX() const { return m_OffsetX; }
		float GetOffsetY() const { return m_OffsetY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}

		EventClassType(MouseScrolled)

	private:
		float m_OffsetX, m_OffsetY;
	};


	class MouseButtonEvent : public Event {
	public:
		MouseCode GetMouseButton() const { return m_Button; }

	protected:
		MouseButtonEvent(const MouseCode button)
			: m_Button(button)
		{
		}

		MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public: 
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button)
		{
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EventClassType(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button)
		{
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EventClassType(MouseButtonReleased)
	};
}