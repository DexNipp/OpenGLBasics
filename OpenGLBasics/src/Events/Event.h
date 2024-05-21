#pragma once

#include <string>
#include <functional>

namespace glb {

	enum class EventType {
		None = 0, 
		WindowClose, WindowResize,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseScrolled, MouseButtonPressed, MouseButtonReleased
	};

	#define EventClassType(type) static EventType GetStaticType() { return EventType::type; }\
								 virtual EventType GetEventType() const override { return GetStaticType(); }\
								 virtual const char* GetName() const override { return #type; }

	class Event {
	public:
		bool Handled = false;
		
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
	};

	class EventDispatcher {
		
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) 
			:m_Event(event) 
		{
		}

		template<typename T>
		bool Dispatch(const EventFn<T> func) {

			if (m_Event.GetEventType() == T::GetStaticType()) {

				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}
