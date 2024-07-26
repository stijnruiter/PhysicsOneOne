#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "math.hpp"

void Camera::UpdateView()
{
    m_view = glm::lookAt(m_position, m_position + m_cameraDirection, m_cameraUp);
}

Camera::Camera(unsigned int screenWidth, unsigned int screenHeight, glm::vec3 m_position, glm::vec3 direction)
    : m_position(m_position), m_cameraDirection(glm::normalize(direction)), m_worldUp(0, 1, 0)
{
    glm::vec3 cameraLeft = glm::normalize(glm::cross(m_worldUp, m_cameraDirection));
    m_cameraUp = glm::cross(m_cameraDirection, cameraLeft);

    m_pitch = asin(m_cameraDirection.y);
    float cosPitch = cos(m_pitch);
    m_yaw = abs(cosPitch) < 0.01f ? 0.0f : acos(m_cameraDirection.x / cosPitch);

    UpdateView();
    UpdateFrameBuffer(screenWidth, screenHeight);
}

void Camera::UpdateInput(Window& window, float deltaTime)
{
    float distance = m_velocity * deltaTime;

    if (window.IsKeyPressed(GLFW_KEY_W))
    {
        m_position += m_cameraDirection * distance;
    }
    if (window.IsKeyPressed(GLFW_KEY_S))
    {
        m_position -= m_cameraDirection * distance;
    }

    if (window.IsKeyPressed(GLFW_KEY_A))
    {
        m_position += glm::normalize(glm::cross(m_worldUp, m_cameraDirection)) * distance;
    }
    if (window.IsKeyPressed(GLFW_KEY_D))
    {
        m_position -= glm::normalize(glm::cross(m_worldUp, m_cameraDirection)) * distance;
    }

    if (window.IsKeyPressed(GLFW_KEY_SPACE))
    {
        m_position.y += distance;
    }
    if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        m_position.y -= distance;
    }

    UpdateView();
}

void Camera::UpdateMouse(double x, double y)
{
    if (!m_mouseInitialized)
    {
        m_lastX = x;
        m_lastY = y;
        m_mouseInitialized = true;
        return;
    }

    m_yaw += (float)(x - m_lastX) * m_mouseSensitivity;
    m_pitch -= (float)(m_lastY - y) * m_mouseSensitivity;

    m_pitch = Engine::Math::Clamp(m_pitch, -1.57f, 1.57f); // 1.571 = PI/2

    m_cameraDirection.x = - cos(m_yaw) * cos(m_pitch);
    m_cameraDirection.y = - sin(m_pitch);
    m_cameraDirection.z = - sin(m_yaw) * cos(m_pitch);

    glm::vec3 unitY(0, 1, 0);
    glm::vec3 cameraLeft = glm::normalize(glm::cross(m_worldUp, m_cameraDirection));
    glm::vec3 upLocal = glm::cross(m_cameraDirection, cameraLeft);

    UpdateView();

    m_lastX = x;
    m_lastY = y;
}

void Camera::UpdateFrameBuffer(unsigned int width, unsigned int height)
{
    m_projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
}

const glm::mat4& Camera::GetView() const
{
    return m_view;
}

const glm::mat4& Camera::GetProjection() const
{
    return m_projection;
}
