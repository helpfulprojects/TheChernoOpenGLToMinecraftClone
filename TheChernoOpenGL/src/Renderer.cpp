#include "Renderer.h"

void Renderer::Clear() const
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib) const
{
	va.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va) const
{
	va.Bind();
	GlCall(glDrawArrays(GL_TRIANGLES, 0, va.GetVerticesCount()));
}
