#include "IndexBuffer.h"
#include "ErrorManager.h"

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count): m_Count(count)
{
    GlCall(glGenBuffers(1, &m_RendererID));
    Bind();
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
