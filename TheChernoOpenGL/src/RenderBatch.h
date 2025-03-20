#pragma once
#include <vector>
#include "Block.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
class RenderBatch {
public:
	RenderBatch();
	bool AddVertex(const Vertex& vertex);
	void Draw();
private:
	const static int BATCH_SIZE = 1000;
	std::vector<Vertex> m_Vertices;
	unsigned int m_Va;
	unsigned int m_Vb;
};
