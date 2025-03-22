#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <unordered_map>
#include "Chunk.h"
#include <glm/gtx/hash.hpp>
#include "ThreadPool.h"
#include "ChunkPosition.h"
#include "ChunkPositionHash.h"
class Renderer;
class World {
public:
	~World();
	void UpdateChunksToRender(const glm::vec3& playerPosition);
	void LoadChunksInRenderer(Renderer& renderer, ThreadPool& threadPool);
	void LoadChunkIfHaveTo(const ChunkPosition& chunkPosition, ThreadPool& threadPool, Renderer& renderer);
	void Draw(const Renderer& renderer);
	const static unsigned int RENDER_DISTANCE = 15;
	const static unsigned int MAX_CHUNKS_COUNT = (RENDER_DISTANCE * 2 + 1) * (RENDER_DISTANCE * 2 + 1);
	ChunkPosition m_ChunksToRender[MAX_CHUNKS_COUNT];
	void GetGeneratedChunksFromThreadLoop(ThreadPool& threadPool);
private:
	void GenerateChunkIfHaveTo(const ChunkPosition& chunkPosition, ThreadPool& threadPool);
	Chunk* GetChunk(const ChunkPosition& chunkOrigin);
	void WaitForChunkGeneration(const ChunkPosition& chunkOrigin);
	std::unordered_map<ChunkPosition,Chunk*,ChunkPositionHash> m_Chunks;
	std::vector<ChunkPosition> m_CheckForGeneratedChunks;
};
