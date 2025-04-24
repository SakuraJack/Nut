#include "ntpch.h"

#include "Core/Application.h"
#include "Core/Log.h"

int main()
{
	Nut::Log::Init();
	Nut::Application* app = Nut::CreateApplication();
	app->Run();
	delete app;
}