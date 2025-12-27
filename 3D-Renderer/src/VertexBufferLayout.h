#pragma once
#include <vector>
#include <GL\glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:		   return 4;
		case GL_UNSIGNED_INT:  return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
	VertexBufferElement(unsigned int c, unsigned int t, unsigned char n):
		count(c), type(t), normalized(n) {}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::Push");
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back(VertexBufferElement(count, GL_FLOAT, GL_FALSE));
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back(VertexBufferElement(count, GL_UNSIGNED_INT, GL_FALSE));
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back(VertexBufferElement(count, GL_UNSIGNED_BYTE, GL_TRUE));
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;


	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

};