#pragma once
#include "Block.h"
class VertexBuffer {
public:
	VertexBuffer(const float* data,unsigned int count);
	VertexBuffer(const Vertex* data,unsigned int count);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};
