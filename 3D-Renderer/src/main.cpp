#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

void processInput(GLFWwindow* window);

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource; 
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
			ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
	const char* src = source.c_str();       // returns pointer to the underlying data in the string
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));     // this is because we already linked them
    GLCall(glDeleteShader(fs));

    return program;
}

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

        // create vertex buffer (vbo)
        VertexBuffer vb(positions, sizeof(positions));

        // set the vertex attribute pointers for position
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0));

        // set the vertex attribute pointers for color
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float))));

        // create index buffer (ibo)
        IndexBuffer ib(indicies, 6);

        // use shader program
        ShaderProgramSource shaderSource = ParseShader("res/shaders/basic.shader");
        unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
        GLCall(glUseProgram(shader));

        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        //ASSERT(location != -1);     // couldnt find uniform
        GLCall(glUniform4f(location, 0.0f, 0.3f, 0.9f, 1.0f));

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            float timeValue = glfwGetTime();
            float r = sin(timeValue) / 3.0f + 0.5f;

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.9f, 1.0f));

            GLCall(glBindVertexArray(vao));
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
