#include "Renderer.h"

void Renderer::Clear() const
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader) const
{
	va.Bind();
	GlCall(glDrawArrays(GL_TRIANGLES, 0, va.GetVerticesCount()));
}

void Renderer::Draw(const Chunk& chunk, const Shader& shader) const
{
	chunk.m_Va->Bind();
	GlCall(glDrawElements(GL_TRIANGLES, chunk.m_Ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}
