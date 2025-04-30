#include "ntpch.h"

#include "Core/Application.h"
#include "Core/Log.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Nut::Log::Init();
	Nut::Application* app = Nut::CreateApplication();
	app->Run();
	delete app;
}