#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
    TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
          m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
        float positions[] = 
        {
            // positions             // colors            // textures
            -50.0f, -50.0f, 0.0f,    0.8f, 0.2f, 0.9f,    0.0f, 0.0f,
             50.0f, -50.0f, 0.0f,    0.2f, 0.8f, 0.9f,    1.0f, 0.0f,
             50.0f,  50.0f, 0.0f,    0.9f, 0.9f, 0.2f,    1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f,    0.9f, 0.2f, 0.4f,    0.0f, 1.0f

        };

        unsigned int indicies[] = 
        {
            0, 1, 2,
            2, 3, 0,
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push<float>(3);      // position attribute (x, y, z)
        layout.Push<float>(3);      // color attribute (r, g, b)
        layout.Push<float>(2);      // texture attribute (u, v)
        m_VAO->AddBuffer(*m_VB, layout);

        m_IBO = std::make_unique<IndexBuffer>(indicies, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/glass-tile.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::onUpdate(float deltaTime)
	{
	}

	void TestTexture2D::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind(0);

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
	}

	void TestTexture2D::onImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 1920.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 1920.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}