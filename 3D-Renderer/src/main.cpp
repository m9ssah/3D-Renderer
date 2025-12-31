#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void processInput(GLFWwindow* window);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
            // positions             // colors            // textures
            -50.0f, -50.0f, 0.0f,    0.8f, 0.2f, 0.9f,    0.0f, 0.0f,
             50.0f, -50.0f, 0.0f,    0.2f, 0.8f, 0.9f,    1.0f, 0.0f,
             50.0f,  50.0f, 0.0f,    0.9f, 0.9f, 0.2f,    1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f,    0.9f, 0.2f, 0.4f,    0.0f, 1.0f

        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0,
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        // remember that opengl is like a state machine, so everything you generate is assigned to a unique id which represents that specific shader
        // then we can use that id to bind the shader and use it in our render loop

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));
        
        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));      // sizeof(float) * 4 * 9

        VertexBufferLayout layout;
        layout.Push<float>(3);  // position attribute (x, y, z)
        layout.Push<float>(3);  // color attribute (r, g, b)
        layout.Push<float>(2);  // texture attribute (u, v)
        va.AddBuffer(vb, layout);

        // create index buffer (ibo)
        IndexBuffer ib(indicies, 6);

        glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        // use shader program
        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.9f, 1.0f);

        Texture texture("res/textures/glass-tile.jpg");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture0", 0);
        
        Texture texture2("res/textures/woaw.jpg");
        texture2.Bind(1);
        shader.SetUniform1i("u_Texture1", 1);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.Fonts->AddFontDefault();
        //io.Fonts->Build();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        glm::vec3 translationA (200, 200, 0);
        glm::vec3 translationB (400, 200, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // first uniform
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                texture.Bind(0);
                texture2.Bind(1);
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                texture.Bind(0);
                texture2.Bind(1);
                shader.SetUniformMat4f("u_MVP", mvp);

                renderer.Draw(va, ib, shader);
            }

            {
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 1920.0f);
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 1920.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
