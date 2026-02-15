#pragma once

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <memory>

class Skybox
{
public:
	Skybox(const std::vector<std::string>& faces);
	~Skybox();

	void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
	unsigned int LoadCubeMap(const std::vector<std::string>& faces);

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<Shader> m_Shader;

	unsigned int m_CubeMapID;
};