#include "VertexArray.h"
#include "ErrorManager.h"

VertexArray::VertexArray()
{
	GlCall(glCreateVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const VertexBufferElement& element = elements[i];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}
