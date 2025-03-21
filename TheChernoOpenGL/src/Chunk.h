#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <array>
#include <iostream>
#include "perlin_noise/PerlinNoise.hpp"
class Renderer;
class Chunk {
public:
	Chunk();
	Chunk(glm::vec3 position);
	~Chunk();
	std::vector<Vertex> GetChunkBlocksVertecies(const Chunk& leftChunk,const Chunk& rightChunk,const Chunk& frontChunk,const Chunk& backChunk) const;
	inline glm::vec3 GetPosition() const { return m_Position; }
	void Draw(const Renderer& renderer);
	const static int HEIGHT = 50;
	const static int WIDTH = 16;
	const static int DEPTH = 16;
	const static siv::PerlinNoise::seed_type SEED = 123456u;
	const siv::PerlinNoise perlin{ SEED };
private:
	glm::vec3 m_Position;
	int_fast8_t GetBlock(int x, int y, int z)const;
	inline int PositionToIndex(int x,int y, int z)const { return x + (z * Chunk::WIDTH) + (y * Chunk::WIDTH * Chunk::DEPTH); }
	std::vector<Vertex> GenerateBlockVertices(const glm::vec3& position, int_fast8_t type, const bool* neighbours)const;
	int_fast8_t m_Blocks[HEIGHT*WIDTH*DEPTH];
};
