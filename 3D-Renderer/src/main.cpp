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
            // positions              // colors
            -0.5f, -0.5f,  0.0f,      0.8f, 0.2f, 0.9f,
             0.5f, -0.5f,  0.0f,      0.2f, 0.8f, 0.9f,
             0.5f,  0.5f,  0.0f,      0.9f, 0.9f, 0.2f,
            -0.5f,  0.5f,  0.0f,      0.9f, 0.2f, 0.4f

        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0,
        };

        // remember that opengl is like a state machine, so everything you generate is assigned to a unique id which represents that specific shader
        // then we can use that id to bind the shader and use it in our render loop

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));
        
        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));

        VertexBufferLayout layout;
        layout.Push<float>(3);  // Position attribute (x, y, z)
        layout.Push<float>(3);  // Color attribute (r, g, b)
        va.AddBuffer(vb, layout);

        // create index buffer (ibo)
        IndexBuffer ib(indicies, 6);

        // use shader program
        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.9f, 1.0f);

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
