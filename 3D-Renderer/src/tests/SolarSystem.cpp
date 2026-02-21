#include "SolarSystem.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include <cmath>
#include <chrono>

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
        m_Rotation(0.0f), m_AutoRotate(true), m_RotationSpeed(45.0f), m_ElapsedTime(0.0f),
        m_QueryFrontIndex(0), m_QueryReady(false),
        m_CpuFrameTimeMs(0.0f), m_GpuFrameTimeMs(0.0f), m_SphereCount(10)
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

        m_Shader = std::make_unique<Shader>("res/shaders/planet.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/4k_ceres_fictional.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);

        std::vector<std::string> skyboxFaces = {
            "res/textures/skybox1/right.jpg",
            "res/textures/skybox1/left.jpg",
            "res/textures/skybox1/top.jpg",
            "res/textures/skybox1/bottom.jpg",
            "res/textures/skybox1/front.jpg",
            "res/textures/skybox1/back.jpg"
        };
        m_Skybox = std::make_unique<Skybox>(skyboxFaces);

        GLCall(glGenQueries(2, m_GpuQueryID));
    }

    SolarSystem::~SolarSystem()
    {
        GLCall(glDeleteQueries(2, m_GpuQueryID));
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
            int k1 = i * (sectorCount + 1);       // current stack start index
            int k2 = k1 + sectorCount + 1;        // next stack start index

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
        // read prev frame timer result from front 
        if (m_QueryReady)
        {
            GLuint64 gpuTimeNs = 0;
            GLCall(glGetQueryObjectui64v(m_GpuQueryID[m_QueryFrontIndex], GL_QUERY_RESULT, &gpuTimeNs));
            m_GpuFrameTimeMs = (float)gpuTimeNs / 1000000.0f;
        }

        // swap front back query indicies
        int queryBackIndex = 1 - m_QueryFrontIndex;

        GLCall(glBeginQuery(GL_TIME_ELAPSED, m_GpuQueryID[queryBackIndex]));


        auto cpuStart = std::chrono::high_resolution_clock::now();

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        Renderer renderer;

        m_View = m_Camera.GetViewMatrix();
        m_Proj = glm::perspective(glm::radians(m_Camera.GetZoom()), 1920.0f / 1080.0f, 0.1f, 100.0f);

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

        m_Shader->SetUniformMat4f("view", m_View);

        int numPositions = 10;
        for (int i = 0; i < m_SphereCount; i++)
        {
            glm::vec3 pos = planetPositions[i % numPositions];
            if (i >= numPositions)
            {
                float row = (float)(i / numPositions);
                pos += glm::vec3(row * 3.0f, 0.0f, 0.0f);
            }

            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation + pos);
            float angle = 20.0f * (float)i;
            model = glm::rotate(model, glm::radians(angle + m_Rotation), glm::vec3(0.2f, 1.0f, 0.3f));
            glm::mat4 mvp = m_Proj * m_View * model;      // order matters
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniform3f("u_LightPos", glm::vec3(0.0f, 0.0f, 0.0f));
            m_Shader->SetUniform3f("u_ViewPos", m_Camera.GetPosition());
            m_Shader->SetUniform3f("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            renderer.DrawSphere(*m_VAO, *m_IBO, *m_Shader);
        }
        m_Skybox->Draw(m_View, m_Proj);

        auto cpuEnd = std::chrono::high_resolution_clock::now();
        m_CpuFrameTimeMs = std::chrono::duration<float, std::milli>(cpuEnd - cpuStart).count();

        GLCall(glEndQuery(GL_TIME_ELAPSED));

        m_QueryFrontIndex = queryBackIndex;
        m_QueryReady = true;
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

        ImGui::Separator();

        // stress test controls
        ImGui::Text("Performance Monitor");
        ImGui::SliderInt("Sphere Count", &m_SphereCount, 1, 1000);

        ImGui::Separator();

        ImGui::Text("CPU frame time: %.3f ms", m_CpuFrameTimeMs);
        ImGui::Text("GPU frame time: %.3f ms", m_GpuFrameTimeMs);
        ImGui::Text("Draw calls:     %d (spheres) + 1 (skybox)", m_SphereCount);

        float totalMs = 1000.0f / ImGui::GetIO().Framerate;
        ImGui::Text("Total frame:    %.3f ms (%.1f FPS)", totalMs, ImGui::GetIO().Framerate);

        ImGui::Separator();
        ImGui::Text("CPU vs GPU:");
        float maxTime = (m_CpuFrameTimeMs > m_GpuFrameTimeMs) ? m_CpuFrameTimeMs : m_GpuFrameTimeMs;
        if (maxTime < 0.001f) maxTime = 0.001f;
        ImGui::ProgressBar(m_CpuFrameTimeMs / maxTime, ImVec2(-1, 0), "CPU");
        ImGui::ProgressBar(m_GpuFrameTimeMs / maxTime, ImVec2(-1, 0), "GPU");

        if (m_CpuFrameTimeMs > m_GpuFrameTimeMs)
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), ">> CPU-BOUND (bottleneck: draw calls / matrix math)");
        else
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 1.0f, 1.0f), ">> GPU-BOUND (bottleneck: shader / fragment count)");
    }

}