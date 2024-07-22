#include "gl_core.h"
#include "Logger.h"
#include <format>

void Engine::GLClearErrors()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		Logger::LogError(std::format("GLERROR still in queue {:#06x}", err));
		DEBUGBREAK();
	}
}

bool Engine::GLCheckErrors()
{
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR)
	{
		// TODO: Format err code to readable error message
		Logger::LogError(std::format("GLERROR {:#06x}", err));
		return false;
	}
	return true;
}
