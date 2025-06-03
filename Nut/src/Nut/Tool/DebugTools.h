#pragma once
#include <glad/glad.h>
#include <string>

namespace Nut {
	class OpenGLDebugTools
	{
	public:
		static void EnableOpenGLDebugMessage();
		static void DisableOpenGLDebugMessage();
	};
}