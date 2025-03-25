#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <array>
#include <iostream>
#include "perlin_noise/PerlinNoise.hpp"
#include "ChunkPosition.h"
class Renderer;
class Chunk {
public:
	Chunk();
	void SpawnLeaveAtRandom(float x, float y, float z, int chance=100);
	Chunk(const ChunkPosition& position);
	~Chunk();
	void GetChunkBlocksVertecies(std::vector<Vertex>& terrainVertices, std::vector<Vertex>& waterVertices,const Chunk* rightChunk, const Chunk* leftChunk, const Chunk* frontChunk, const Chunk* backChunk) const;
	inline ChunkPosition GetPosition() const { return m_Position; }
	const static int HEIGHT = 256;
	const static int WIDTH = 16;
	const static int DEPTH = 16;
	const static siv::PerlinNoise::seed_type SEED = 123994u;
	static double GetContinentalness(float x, float z);
	const static siv::PerlinNoise perlin;
	const static float m_Continentalness;
private:
	static bool IsNeighbourDifferentAndSolid(int_fast8_t currentBlockType,int_fast8_t neighbourType);
	ChunkPosition m_Position;
	int_fast8_t GetBlock(int x, int y, int z);
	inline int PositionToIndex(int x,int y, int z)const { 
		if (x < 0) x = 0;
		if (x >= Chunk::WIDTH) x = Chunk::WIDTH-1;
		if (z < 0) z = 0;
		if (z >= Chunk::DEPTH) z = Chunk::DEPTH-1;
		if (y < 0) y = 0;
		if (y >= Chunk::HEIGHT) y = Chunk::HEIGHT-1;
		return x + (z * Chunk::WIDTH) + (y * Chunk::WIDTH * Chunk::DEPTH); 
	}
	std::vector<Vertex> GenerateBlockVertices(const Vec3& position, int_fast8_t type, const bool* neighbours) const;
	int_fast8_t m_Blocks[HEIGHT * WIDTH * DEPTH] = {(int_fast8_t)BlockType::Air};
};
