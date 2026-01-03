#include "CubeWorld.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
    CubeWorld::CubeWorld()
        : m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))), m_Proj(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f)),
        m_Translation(0.0f, 0.0f, 0.0f),
        m_Rotation(0.0f), m_AutoRotate(true), m_RotationSpeed(45.0f), m_Verticies(36)
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

        m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/glass-tile.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    CubeWorld::~CubeWorld()
    {
    }

    void CubeWorld::onUpdate(float deltaTime)
    {
        if (m_AutoRotate)
        {
            m_Rotation += m_RotationSpeed * deltaTime;
            if (m_Rotation > 360.0f)
                m_Rotation -= 360.0f;
        }
    }

    void CubeWorld::onRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind(0);

        glm::vec3 cubePositions[] =
        {
            glm::vec3( 0.0f,   0.0f,   0.0f),
            glm::vec3( 2.0f,   5.0f,  -15.0f),
            glm::vec3(-1.5f,  -2.2f,  -2.5f),
            glm::vec3(-3.8f,  -2.0f,  -12.3f),
            glm::vec3( 2.4f,  -0.4f,  -3.5f),
            glm::vec3(-1.7f,   3.0f,  -7.5f),
            glm::vec3( 1.3f,  -2.0f,  -2.5f),
            glm::vec3( 1.5f,   2.0f,  -2.5f),
            glm::vec3( 1.5f,   0.2f,  -1.5f),
            glm::vec3(-1.3f,   1.0f,  -1.5f)
        };

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation + cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle + m_Rotation), glm::vec3(0.2f, 1.0f, 0.3f));
            glm::mat4 mvp = m_Proj * m_View * model;      // order matters
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.DrawCube(*m_VAO, m_Verticies, *m_Shader);
        }
    }

    void CubeWorld::onImGuiRender()
    {
        ImGui::SliderFloat3("Translation", &m_Translation.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 360.0f);
        ImGui::Checkbox("Auto Rotate", &m_AutoRotate);

        ImGui::Separator();

        if (m_AutoRotate)
            ImGui::SliderFloat("Rotation Speed", &m_RotationSpeed, 0.0f, 180.0f);

        ImGui::Separator();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}