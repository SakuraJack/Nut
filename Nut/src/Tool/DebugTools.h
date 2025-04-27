#pragma once
#include <glad/glad.h>
#include <string>
#include "Core/Log.h"

namespace Nut {
	class OpenGLDebugTools
	{
	public:
		static void EnableOpenGLDebugMessage();
		static void DisableOpenGLDebugMessage();
	};
}