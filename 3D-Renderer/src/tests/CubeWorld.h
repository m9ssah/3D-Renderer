#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "CameraController.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

// goal: solar system sim

namespace test {

	class CubeWorld : public Test
	{
	public:
		CubeWorld();
		~CubeWorld();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		GLint m_Verticies;
		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation;
		float m_Rotation;
		bool m_AutoRotate;
		float m_RotationSpeed;
		float m_ElapsedTime;
	};

}