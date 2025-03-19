#include "VertexBuffer.h"
#include "ErrorManager.h"

VertexBuffer::VertexBuffer(const float* data,unsigned int count): m_Count(count)
{
	GlCall(glGenBuffers(1, &m_RendererID));
	Bind();
	glBufferData(GL_ARRAY_BUFFER, count*sizeof(float), data, GL_STATIC_DRAW);
	Unbind();
}

VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
