#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
            // positions                // textures
            -0.5f, -0.5f,  0.0f,        0.0f, 0.0f,
             0.5f, -0.5f,  0.0f,        1.0f, 0.0f,
             0.5f,  0.5f,  0.0f,        1.0f, 1.0f,
            -0.5f,  0.5f,  0.0f,        0.0f, 1.0f,

        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0,
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));      // TEMP FIX
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
        //layout.Push<float>(3);  // color attribute (r, g, b)
        layout.Push<float>(2);  // texture attribute (u, v)
        va.AddBuffer(vb, layout);

        // create index buffer (ibo)
        IndexBuffer ib(indicies, 6);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        // use shader program
        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.9f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

        Texture texture("res/textures/glass-tile.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            /* Render here */
            renderer.Clear();

            float timeValue = glfwGetTime();
            float r = sin(timeValue) / 3.0f + 0.5f;

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.9f, 1.0f);

            renderer.Draw(va, ib, shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
