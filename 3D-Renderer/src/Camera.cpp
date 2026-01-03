#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{

}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{

}

void Camera::SetProjection(float left, float right, float bottom, float top)
{

}

void Camera::RecalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}