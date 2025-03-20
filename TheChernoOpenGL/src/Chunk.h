#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <array>
#include "Renderer.h"
class Chunk {
public:
	Chunk();
	Chunk(glm::vec3 position);
	~Chunk();
	void UpdateVertices(const Chunk& leftChunk,const Chunk& rightChunk,const Chunk& frontChunk,const Chunk& backChunk);
	void Draw(const Renderer& renderer);
	const static unsigned int HEIGHT = 10;
	const static unsigned int WIDTH = 16;
	const static unsigned int DEPTH = 16;
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	void GenerateBlockVerts(const glm::vec3& position, const BlockType& type);
	BlockType* m_Blocks;
	glm::vec3 m_Position;
};
