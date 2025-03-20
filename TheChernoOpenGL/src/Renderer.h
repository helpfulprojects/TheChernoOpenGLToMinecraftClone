#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "RenderBatch.h"
class Renderer {
public:
	Renderer();
	~Renderer();
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib) const;
	void Draw(const VertexArray& va) const;
	void Draw() const;
	void AddVertex(const Vertex& vertex);
	void AddVertices(const std::vector<Vertex>& vertices);
private:
	std::vector<RenderBatch*> m_Batches;
};
