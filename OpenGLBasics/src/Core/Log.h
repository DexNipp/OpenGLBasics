#pragma once
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace glb {

	class Log {
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#define GLB_CRITICAL(...)     ::glb::Log::GetLogger()->critical(__VA_ARGS__)
#define GLB_ERROR(...)	      ::glb::Log::GetLogger()->error(__VA_ARGS__)
#define GLB_WARN(...)	      ::glb::Log::GetLogger()->warn(__VA_ARGS__)
#define GLB_INFO(...)	      ::glb::Log::GetLogger()->info(__VA_ARGS__)
#define GLB_TRACE(...)	      ::glb::Log::GetLogger()->trace(__VA_ARGS__)