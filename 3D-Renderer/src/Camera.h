#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	// vector vals
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

	// scalar vals
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	void SetProjection(float left, float right, float bottom, float top);

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec3& position) { m_Position = position; updateCameraVectors(); }

	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) { m_Rotation = rotation; updateCameraVectors(); }

	float GetYaw() const { return m_Yaw; }
	void SetYaw(float yaw) { m_Yaw = yaw; updateCameraVectors(); }

	float GetPitch() const { return m_Pitch; }
	void SetPitch(float pitch) { m_Pitch = pitch; updateCameraVectors(); }
	
	float GetSensitivity() const { return m_Sensitivity; }
	void SetSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }

	float GetZoom() const { return m_Zoom; }
	void SetZoom(float zoom) { m_Zoom = zoom; }

	float GetSpeed() const { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }

	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	void updateCameraVectors();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
	glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };

	float m_Rotation = 0.0f;
	float m_Yaw;
	float m_Pitch;
	float m_Sensitivity;
	float m_Zoom;
	float m_Speed = 2.5f;


};
