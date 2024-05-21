#pragma once

#include "Core/Application.h"
#include "Log.h"

int main(int argc, char** argv) {

	glb::Log::Init();
	GLB_INFO("Log::Init in EntryPoint.h\n");

	auto app = glb::Application::CreateApplication();
	app->Run();
	delete app;
}