#include "ntpch.h"
#include "DebugTools.h"

void Nut::OpenGLDebugTools::EnableOpenGLDebugMessage()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				NUT_ERROR_TAG("OpenGL", "OpenGL Debug Message: {0}", message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				NUT_WARN_TAG("OpenGL", "OpenGL Debug Message: {0}", message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				NUT_INFO_TAG("OpenGL", "OpenGL Debug Message: {0}", message);
				break;
			default:
				NUT_TRACE_TAG("OpenGL", "OpenGL Debug Message: {0}", message);
				break;
			}
		}, nullptr);
}

void Nut::OpenGLDebugTools::DisableOpenGLDebugMessage()
{
	glDisable(GL_DEBUG_OUTPUT);
	glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
