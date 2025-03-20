#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <array>
class Chunk {
public:
	Chunk();
	~Chunk();
	IndexBuffer* m_Ib;
	VertexArray* m_Va;
	void UpdateVertexArray();
private:
	std::array<Vertex,24> GenerateBlockVerts(glm::vec3 position, BlockType type);
	const static unsigned int HEIGHT = 50;
	const static unsigned int WIDTH = 16;
	const static unsigned int DEPTH = 16;
	VertexBuffer* m_Vb;
	BlockType m_Blocks[WIDTH * DEPTH * HEIGHT];
};
