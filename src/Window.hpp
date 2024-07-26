#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>

class Window
{
private:
	GLFWwindow* m_window;

	int m_width;
	int m_height;
	std::string m_title;

private:
	bool InitializeGLFW();
	bool InitializeGlad();

	void AttachEventCallbacks();
	
	void(*m_callbackFrameBuffer)(unsigned int width, unsigned int height);
	void(*m_callbackCursorPos)(double cursorX, double cursorY);
	void(*m_callbackKey)(int key, int scancode, int action, int mods);

public:
	Window(int width, int height, std::string title);
	~Window();

	void SetMouseCursor(bool enabled);
	bool ShouldClose();
	void SwapBuffers();

	void Close();

	void SetFrameBufferCallback(void(*callback)(unsigned int width, unsigned int height)) { m_callbackFrameBuffer = callback; }
	void SetCursorPosCallback(void(*callback)(double cursorX, double cursorY)) { m_callbackCursorPos = callback; }
	void SetKeyCallback(void(*callback)(int key, int scancode, int action, int mods)) { m_callbackKey = callback; }

	bool IsKeyPressed(int key) const;

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	float GetAspectRatio() const { return m_height == 0 ? 1.0f : (float)m_width / m_height; } // Window is probably hidden, stop rendering;

};