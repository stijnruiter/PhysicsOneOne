#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "math.hpp"

void Camera::UpdateView()
{
    view = glm::lookAt(position, position + cameraDirection, cameraUp);
}

Camera::Camera(glm::vec3 position, glm::vec3 direction)
    : position(position), cameraDirection(glm::normalize(direction)), worldUp(0, 1, 0)
{
    glm::vec3 cameraLeft = glm::normalize(glm::cross(worldUp, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraLeft);

    pitch = asin(cameraDirection.y);
    float cosPitch = cos(pitch);
    yaw = abs(cosPitch) < 0.01f ? 0.0f : acos(cameraDirection.x / cosPitch);

    UpdateView();
}

void Camera::UpdateInput(GLFWwindow* window, float deltaTime)
{
    float distance = velocity * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += cameraDirection * distance;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= cameraDirection * distance;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position += glm::normalize(glm::cross(worldUp, cameraDirection)) * distance;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position -= glm::normalize(glm::cross(worldUp, cameraDirection)) * distance;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        position.y += distance;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        position.y -= distance;
    }

    UpdateView();
}

void Camera::UpdateMouse(double x, double y)
{
    if (!mouseInitialized)
    {
        lastX = x;
        lastY = y;
        mouseInitialized = true;
        return;
    }

    yaw += (float)(x - lastX) * mouseSensitivity;
    pitch -= (float)(lastY - y) * mouseSensitivity;

    pitch = Engine::Math::Clamp(pitch, -1.57f, 1.57f); // 1.571 = PI/2

    cameraDirection.x = - cos(yaw) * cos(pitch);
    cameraDirection.y = - sin(pitch);
    cameraDirection.z = - sin(yaw) * cos(pitch);

    glm::vec3 unitY(0, 1, 0);
    glm::vec3 cameraLeft = glm::normalize(glm::cross(worldUp, cameraDirection));
    glm::vec3 upLocal = glm::cross(cameraDirection, cameraLeft);

    UpdateView();

    lastX = x;
    lastY = y;
}

const glm::mat4& Camera::GetView() const
{
    return view;
}
