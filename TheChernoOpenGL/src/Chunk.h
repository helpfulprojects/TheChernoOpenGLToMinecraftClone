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
	Chunk(const ChunkPosition& position);
	~Chunk();
	void GetChunkBlocksVertecies(std::vector<Vertex>& terrainVertices, std::vector<Vertex>& waterVertices,const Chunk* rightChunk, const Chunk* leftChunk, const Chunk* frontChunk, const Chunk* backChunk) const;
	inline ChunkPosition GetPosition() const { return m_Position; }
	const static int HEIGHT = 256;
	const static int WIDTH = 16;
	const static int DEPTH = 16;
	const static siv::PerlinNoise::seed_type SEED = 323456u;
	static double GetContinentalness(float x, float z);
	const static siv::PerlinNoise perlin;
	const static float m_Continentalness;
private:
	static bool IsNeighbourDifferentAndSolid(int_fast8_t currentBlockType,int_fast8_t neighbourType);
	ChunkPosition m_Position;
	int_fast8_t GetBlock(int x, int y, int z)const;
	inline int PositionToIndex(int x,int y, int z)const { return x + (z * Chunk::WIDTH) + (y * Chunk::WIDTH * Chunk::DEPTH); }
	std::vector<Vertex> GenerateBlockVertices(const Vec3& position, int_fast8_t type, const bool* neighbours) const;
	int_fast8_t m_Blocks[HEIGHT*WIDTH*DEPTH];
};
