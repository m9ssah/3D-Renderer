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
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestTexture3D.h"
#include "tests/CubeWorld.h"

// Camera - global or accessible pointer for callbacks
Camera* g_Camera = nullptr;

float g_DeltaTime = 0.0f;

bool g_FirstMouse = true;
float g_LastX = 1920.f / 2;
float g_LastY = 1080.0f / 2;

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.Fonts->AddFontDefault();
        //io.Fonts->Build();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* menuTest = new test::TestMenu(currentTest);
        currentTest = menuTest;

        menuTest->RegisterTest<test::TestClearColor>("Clear Color");
        menuTest->RegisterTest<test::TestTexture2D>("2D Texture");
        menuTest->RegisterTest<test::TestTexture3D>("3D Texture");
        menuTest->RegisterTest<test::CubeWorld>("Cube World");


        float lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = (float)glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            g_DeltaTime = deltaTime;

            test::CubeWorld* cubeWorld = dynamic_cast<test::CubeWorld*>(currentTest);
            if (cubeWorld)
            {
                g_Camera = &cubeWorld->GetCamera();
            }
            else
            {
                g_Camera = nullptr;
            }

            processInput(window);

            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->onUpdate(deltaTime);
                currentTest->onRender();
                ImGui::Begin("Test");
                if (currentTest != menuTest && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = menuTest;
                }
                currentTest->onImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != menuTest)
        {
            delete menuTest;
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

    if (g_Camera)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            g_Camera->onKeyboard(FORWARD, g_DeltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            g_Camera->onKeyboard(BACKWARD, g_DeltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            g_Camera->onKeyboard(LEFT, g_DeltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            g_Camera->onKeyboard(RIGHT, g_DeltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!g_Camera)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (g_FirstMouse)
    {
        g_LastX = xpos;
        g_LastY = ypos;
        g_FirstMouse = false;
    }

    float xoffset = xpos - g_LastX;
    float yoffset = g_LastY - ypos; // reversed since y-coordinates go from bottom to top

    g_LastX = xpos;
    g_LastY = ypos;

    g_Camera->onMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (g_Camera)
        g_Camera->onScroll(static_cast<float>(yoffset));
}
