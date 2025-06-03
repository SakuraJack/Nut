#pragma once

#include "Core/Application.h"

extern Nut::Application* Nut::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

namespace Nut {

	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			InitializeCore();
			Application* app = CreateApplication(argc, argv);
			app->Run();
			delete app;
			ShutdownCore();
		}
		return 0;
	}
}

#if NT_DIST && NT_PLATFORM_WINDOWS

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, PSTR cmdline, int cmdshow)
{
	return Nut::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Nut::Main(argc, argv);
}

#endif