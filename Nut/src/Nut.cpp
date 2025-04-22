#include "ntpch.h"

#include "Core/Application.h"

int main()
{
	Nut::Application* app = Nut::CreateApplication();
	app->Run();
	delete app;
}