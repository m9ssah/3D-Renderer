#include "SolarSystem.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace test
{
    SolarSystem::SolarSystem()
        : m_Camera({ 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 0.0f }, -90.0f, 0.0f),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))), 
        m_Proj(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f)),
        m_Translation(0.0f, 0.0f, 0.0f),
        m_Rotation(0.0f), m_AutoRotate(true), m_RotationSpeed(45.0f), m_ElapsedTime(0.0f)
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        GenerateSphere(0.5f, 36, 18, vertices, indices);

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));

        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(vertices.data(), (unsigned int)(vertices.size() * sizeof(float)));
        VertexBufferLayout layout;
        layout.Push<float>(3);      // position attr (x, y, z)  
        layout.Push<float>(3);      // color attr (r, g, b)
        layout.Push<float>(2);      // texture attr (u, v)
        m_VAO->AddBuffer(*m_VB, layout);

        m_IBO = std::make_unique<IndexBuffer>(indices.data(), (unsigned int)indices.size());

        m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/4k_ceres_fictional.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    SolarSystem::~SolarSystem()
    {
    }

    void SolarSystem::GenerateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
        std::vector<float>& vertices, std::vector<unsigned int>& indices)
    {

        float sectorStep = 2.0f * (float)M_PI / (float)sectorCount;
        float stackStep  = (float)M_PI / (float)stackCount;

        for (unsigned int i = 0; i <= stackCount; ++i)
        {
            float stackAngle = (float)M_PI / 2.0f - (float)i * stackStep;   // from pi/2 to -pi/2
            float xy = radius * cosf(stackAngle);
            float z  = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                float sectorAngle = (float)j * sectorStep;       // from 0 to 2pi

                // pos
                float x = xy * cosf(sectorAngle);       // rcosucosv
                float y = xy * sinf(sectorAngle);       // rcosusinv

                // normal
                float nx = x / radius;
                float ny = y / radius;
                float nz = z / radius;

                // tex coords
                float u = (float)j / (float)sectorCount;
                float v = (float)i / (float)stackCount;

                // pos(3) + color/normal(3) + texcoord(2)  = 8 floats per vertex
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                vertices.push_back(nx);
                vertices.push_back(ny);
                vertices.push_back(nz);
                vertices.push_back(u);
                vertices.push_back(v);
            }
        }

        // 2 triangles per quad, except at the poles where each quad degenerates to one triangle.
        for (int i = 0; i < stackCount; ++i)
        {
            unsigned int k1 = i * (sectorCount + 1);       // current stack start index
            unsigned int k2 = k1 + sectorCount + 1;        // next stack start index

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // first triangle of the quad (skip at top pole where it degenerates)
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // second triangle of the quad (skip at bottom pole where it degenerates)
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }

    void SolarSystem::onUpdate(float deltaTime)
    {
        m_ElapsedTime += deltaTime;
        
        if (m_AutoRotate)
        {
            m_Rotation += m_RotationSpeed * deltaTime;
            if (m_Rotation > 360.0f)
                m_Rotation -= 360.0f;
        }
    }

    void SolarSystem::onRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind(0);

        glm::vec3 planetPositions[] =
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

        m_View = m_Camera.GetViewMatrix();
        m_Proj = glm::perspective(glm::radians(m_Camera.GetZoom()), 1920.0f / 1080.0f, 0.1f, 100.0f);
        m_Shader->SetUniformMat4f("view", m_View);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation + planetPositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle + m_Rotation), glm::vec3(0.2f, 1.0f, 0.3f));
            glm::mat4 mvp = m_Proj * m_View * model;      // order matters
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.DrawSphere(*m_VAO, *m_IBO, *m_Shader);
        }
    }

    void SolarSystem::onImGuiRender()
    {
        ImGui::SliderFloat3("Translation", &m_Translation.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 360.0f);
        ImGui::Checkbox("Auto Rotate", &m_AutoRotate);

        ImGui::Separator();

        if (m_AutoRotate)
            ImGui::SliderFloat("Rotation Speed", &m_RotationSpeed, 0.0f, 180.0f);

        ImGui::Separator();

        glm::vec3 camPos = m_Camera.GetPosition();
        if (ImGui::SliderFloat3("Camera Position", &camPos.x, -20.0f, 20.0f))
            m_Camera.SetPosition(camPos);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}