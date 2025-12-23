#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogError()
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL Error: " << error << std::endl;
        return false;
    }
    return true;
}

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
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
	glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);     // this is because we already linked them
	glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
   		-0.5f,  0.5f
	};

    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };

    // remember that opengl is like a state machine, so everything you generate is assigned to a unique id which represents that specific shader
	// then we can use that id to bind the shader and use it in our render loop

    // create vertex buffer (vbo)
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // start by copying vertecies array in a buffer so opengl can use it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	// set the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// create index buffer (ibo)
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indicies, GL_STATIC_DRAW);

    // use shader program
	ShaderProgramSource shaderSource = ParseShader("res/shaders/basic.shader");
    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        ASSERT(GLLogError());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
