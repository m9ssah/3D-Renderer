#pragma once

#include "Camera.h"

class CameraController
{
public:
	float GetSpeed() const { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }
private:

	float m_AspectRatio;
	float m_ZoomLevel = 1.0f;
	Camera camera;

	bool m_Rotation;

	glm::vec3  m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CameraRotation = 0.0f;		// anti clockwise
	float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpped = 90.0f;

	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	float m_Speed;

};