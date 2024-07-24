#pragma once

#include <glm/glm.hpp>
#include <GLFW\glfw3.h>

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

	void UpdateView();

public:
	Camera(glm::vec3 position, glm::vec3 target);

	void UpdateInput(GLFWwindow* window, float deltaTime);
	void UpdateMouse(double x, double y);

	const glm::mat4& GetView() const;
};