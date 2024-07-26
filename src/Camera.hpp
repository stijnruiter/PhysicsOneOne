#pragma once

#include <glm/glm.hpp>
#include "Window.hpp"

class Camera
{
private:
	glm::vec3 position;

	glm::vec3 worldUp;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;

	float pitch = 0.0;
	float yaw = 0.0;

	float velocity = 1.0f; // m/s
	float mouseSensitivity = 0.001f; // rad/px

	double lastX, lastY;
	bool mouseInitialized = false;

	glm::mat4 view = glm::mat4(1.0);
	glm::mat4 m_projection = glm::mat4(1.0);

	void UpdateView();

public:
	Camera(unsigned int screenWidth, unsigned int screenHeight, glm::vec3 position, glm::vec3 target);

	void UpdateInput(Window& window, float deltaTime);
	void UpdateMouse(double x, double y);
	void UpdateFrameBuffer(unsigned int width, unsigned int height);

	const glm::mat4& GetView() const;
	const glm::mat4& GetProjection() const;
};