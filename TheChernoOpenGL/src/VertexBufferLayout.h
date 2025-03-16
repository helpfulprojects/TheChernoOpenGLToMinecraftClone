#pragma once
#include <vector>
#include "Renderer.h"
#include <GL/glew.h>
struct VertexBufferElement {
	unsigned int count;
	unsigned int type;
	unsigned char normalized;
	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
		case GL_UNSIGNED_INT: return sizeof(GLuint);
		case GL_FLOAT: return sizeof(GLfloat);
		case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout(): m_Stride(0) {};
	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}
	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void Push<char>(unsigned int count) {
		m_Elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline const unsigned int GetStride() const { return m_Stride;  }
};
