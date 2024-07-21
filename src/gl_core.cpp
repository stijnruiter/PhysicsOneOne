#include "gl_core.h"
#include "Logger.h"
#include <format>

void Engine::GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool Engine::GLCheckErrors()
{
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR)
	{
		// TODO: Format err code to readable error message
		Logger::LogError(std::format("Glerror {:#04x}", err));
		return false;
	}
	return true;
}
