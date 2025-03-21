#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <unordered_map>
#include "Chunk.h"
#include <glm/gtx/hash.hpp>
#include "ThreadPool.h"
class Renderer;
class World {
public:
	~World();
	void UpdateChunksToRender(const glm::vec3& playerPosition);
	void LoadChunksInRenderer(Renderer& renderer, ThreadPool& threadPool);
	void LoadChunkIfHaveTo(const glm::vec3& chunkPosition, ThreadPool& threadPool, Renderer& renderer);
	void Draw(const Renderer& renderer);
	const static unsigned int RENDER_DISTANCE = 15;
	const static unsigned int MAX_CHUNKS_COUNT = (RENDER_DISTANCE * 2 + 1) * (RENDER_DISTANCE * 2 + 1);
	glm::vec3 m_ChunksToRender[MAX_CHUNKS_COUNT];
	void GetGeneratedChunksFromThreadLoop(ThreadPool& threadPool);
private:
	void GenerateChunkIfHaveTo(const glm::vec3& chunkPosition, ThreadPool& threadPool);
	Chunk* GetChunk(const glm::vec3& chunkOrigin);
	void WaitForChunkGeneration(glm::vec3 chunkOrigin);
	std::unordered_map<glm::vec3,Chunk*> m_Chunks;
	std::vector<glm::vec3> m_CheckForGeneratedChunks;
};
