#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>

// goal: solar system sim

namespace test {

	class SolarSystem : public Test
	{
	public:
		SolarSystem();
		~SolarSystem();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

		Camera& GetCamera() { return m_Camera; }
	private:
		void GenerateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
			std::vector<float>& vertices, std::vector<unsigned int>& indices);

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		Camera m_Camera;
		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation;
		float m_Rotation;
		bool m_AutoRotate;
		float m_RotationSpeed;
		float m_ElapsedTime;
	};
}