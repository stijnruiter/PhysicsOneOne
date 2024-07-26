#pragma once

#include <glm/glm.hpp>
#include "Window.hpp"

class Camera
{
private:
	glm::vec3 m_position;

	glm::vec3 m_worldUp;
	glm::vec3 m_cameraDirection;
	glm::vec3 m_cameraUp;

	float m_pitch = 0.0;
	float m_yaw = 0.0;

	float m_velocity = 1.0f; // m/s
	float m_mouseSensitivity = 0.001f; // rad/px

	double m_lastX, m_lastY;
	bool m_mouseInitialized = false;

	glm::mat4 m_view = glm::mat4(1.0);
	glm::mat4 m_projection = glm::mat4(1.0);

private:
	void UpdateView();

public:
	Camera(unsigned int screenWidth, unsigned int screenHeight, glm::vec3 m_position, glm::vec3 target);

	void UpdateInput(Window& window, float deltaTime);
	void UpdateMouse(double x, double y);
	void UpdateFrameBuffer(unsigned int width, unsigned int height);

	const glm::mat4& GetView() const;
	const glm::mat4& GetProjection() const;
};