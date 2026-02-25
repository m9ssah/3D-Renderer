cmake_minimum_required(VERSION 3.14)
project(3D-Renderer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# src files
set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/IndexBuffer.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/Skybox.cpp
    src/Texture.cpp
    src/VertexArray.cpp
    src/VertexBuffer.cpp
    src/tests/CubeWorld.cpp
    src/tests/SolarSystem.cpp
    src/tests/Test.cpp
    src/tests/TestClearColor.cpp
    src/tests/TestTexture2D.cpp
    src/tests/TestTexture3D.cpp

    src/vendor/stb_image/stb_image.cpp
    src/vendor/glm/detail/glm.cpp
    src/vendor/imgui/imgui.cpp
    src/vendor/imgui/imgui_demo.cpp
    src/vendor/imgui/imgui_draw.cpp
    src/vendor/imgui/imgui_impl_glfw.cpp
    src/vendor/imgui/imgui_impl_opengl3.cpp
    src/vendor/imgui/imgui_tables.cpp
    src/vendor/imgui/imgui_widgets.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# include dir
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include
)

# preprocessor definition
target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

# link dir
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022
)

# link librariess
target_link_libraries(${PROJECT_NAME} PRIVATE
    glew32s
    glfw3
    opengl32
)

# copy resources to build dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)