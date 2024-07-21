#pragma once
#include <glad/glad.h>


namespace Engine
{
	void GLClearErrors();
	bool GLCheckErrors();
}

#ifdef DEBUG
	// Works for visual studio, might not work for other IDE
	#define DEBUGBREAK() __debugbreak();
#else
	#define DEBUGBREAK()
#endif

#define ASSERT(x) if (!(x)) DEBUGBREAK();

#define GLCHECK(x) Engine::GLClearErrors();	x; ASSERT(Engine::GLCheckErrors())