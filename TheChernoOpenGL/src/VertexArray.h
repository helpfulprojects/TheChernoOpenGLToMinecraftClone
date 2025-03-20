#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddBlockBuffer(const VertexBuffer& vb);

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetVerticesCount() const { return m_VerticesCount; }
private:
	unsigned int m_RendererID;
	unsigned int m_VerticesCount;
};
