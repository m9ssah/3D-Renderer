#include "Skybox.h"

#include "VertexBufferLayout.h"
#include "stb_image/stb_image.h"

#include <iostream>

Skybox::Skybox(const std::vector<std::string>& faces)
	: m_CubeMapID(0)
{
    float skyboxVerticies[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(skyboxVerticies, sizeof(skyboxVerticies));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VBO, layout);

    m_Shader = std::make_unique<Shader>("res/shaders/skybox.shader");

    m_CubeMapID = LoadCubeMap(faces);
}

Skybox::~Skybox()
{
    GLCall(glDeleteTextures(1, &m_CubeMapID));
}

unsigned int Skybox::LoadCubeMap(const std::vector<std::string>& faces)
{
    unsigned int textureID;
    GLCall(glGenTextures(1, &textureID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(0);

    for (int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
            stbi_image_free(data);
        }
        else
        {
            std::cout << "CubeMap texture failed to load";
            stbi_image_free(data);
        }
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    return textureID;
}

void Skybox::Draw(const glm::mat4& view, const glm::mat4& projection)
{
    GLCall(glDepthFunc(GL_LEQUAL));     // change depth func so skybox can passs depth test at max

    m_Shader->Bind();

    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

    m_Shader->SetUniformMat4f("u_View", skyboxView);
    m_Shader->SetUniformMat4f("u_Projection", projection);

    m_VAO->Bind();
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMapID));
    m_Shader->SetUniform1i("u_Skybox", 0);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

    GLCall(glDepthFunc(GL_LESS));       // restore depth
}