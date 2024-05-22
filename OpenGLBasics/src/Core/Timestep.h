#pragma once

namespace glb {

	class Timestep {
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		float GetSeconds() const { return m_Time; }
		float GetMilliSeconds() const { return m_Time * 1000; }

	private:
		float m_Time;
	};
}