#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogError(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogError(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawCube(const VertexArray& va, const GLint verticies, const Shader& shader) const;
    void DrawSphere(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};