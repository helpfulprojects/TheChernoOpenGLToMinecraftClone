#include "Renderer.h"

Renderer::Renderer()
{
	m_Batches.push_back(new RenderBatch());
}

Renderer::~Renderer()
{
	for (RenderBatch* renderBatch : m_Batches) {
		delete renderBatch;
	}
}

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

void Renderer::Draw() const
{
	for (RenderBatch* renderBatch : m_Batches) {
		renderBatch->Draw();
	}
}

void Renderer::AddVertex(const Vertex& vertex)
{
	RenderBatch* lastRenderBatch = m_Batches.back();
	if (!lastRenderBatch->AddVertex(vertex)) {
		m_Batches.push_back(new RenderBatch());
		AddVertex(vertex);
	}
}

void Renderer::AddVertices(const std::vector<Vertex>& vertices)
{
	for (const Vertex& vertex : vertices) {
		AddVertex(vertex);
	}
}
