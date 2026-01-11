#include "TestTexture3D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
    TestTexture3D::TestTexture3D()
        : m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))), m_Proj(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f)),
        m_TranslationA(0.0f, 0.0f, 0.0f), m_TranslationB(1.5f, 0.0f, 0.0f),
        m_RotationA(0.0f), m_RotationB(0.0f), m_AutoRotateA(true), m_AutoRotateB(true), m_RotationSpeed(45.0f), m_Verticies(36)
    {
        float positions[] =
        {   // positions           // colors              // texture
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
                                  
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
                                  
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
                                  
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
                                  
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
                                  
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  0.0f,   0.0f, 1.0f
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push<float>(3);      // position attribute (x, y, z)
        layout.Push<float>(3);      // color attribute (r, g, b)
        layout.Push<float>(2);      // texture attribute (u, v)
        m_VAO->AddBuffer(*m_VB, layout);

        //m_IBO = std::make_unique<IndexBuffer>(indicies, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/glass-tile.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    TestTexture3D::~TestTexture3D()
    {
    }

    void TestTexture3D::onUpdate(float deltaTime)
    {
        if (m_AutoRotateA)
        {
            m_RotationA += m_RotationSpeed * deltaTime;
            if (m_RotationA > 360.0f)
                m_RotationA -= 360.0f;
        }


        if (m_AutoRotateB)
        {
            m_RotationB += m_RotationSpeed * deltaTime;
            if (m_RotationB > 360.0f)
                m_RotationB -= 360.0f;
        }
    }

    void TestTexture3D::onRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind(0);

        {
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationA), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, m_TranslationA);
            model = glm::rotate(model, glm::radians(m_RotationA * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // spin on own axis
            glm::mat4 mvp = m_Proj * m_View * model;      // order matters
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.DrawCube(*m_VAO, m_Verticies, *m_Shader);
        }
        {
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationB), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, m_TranslationB);
            model = glm::rotate(model, glm::radians(m_RotationB * 2.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // spin on own axis
            glm::mat4 mvp = m_Proj * m_View * model;      // order matters
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.DrawCube(*m_VAO, m_Verticies, *m_Shader);
        }
    }

    void TestTexture3D::onImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation A", &m_RotationA, 0.0f, 360.0f);
        ImGui::Checkbox("Auto Rotate A", &m_AutoRotateA);

        ImGui::Separator();

        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation B", &m_RotationB, 0.0f, 360.0f);
        ImGui::Checkbox("Auto Rotate B", &m_AutoRotateB);

        ImGui::Separator();

        if (m_AutoRotateA || m_AutoRotateB)
            ImGui::SliderFloat("Rotation Speed", &m_RotationSpeed, 0.0f, 180.0f);

        ImGui::Separator();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}