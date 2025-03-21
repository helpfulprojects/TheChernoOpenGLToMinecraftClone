#include "World.h"
#include <cmath>
#include "Renderer.h"
#include <chrono>

World::~World()
{
	for (auto itr : m_Chunks){
		delete itr.second;
	}
}

void World::UpdateChunksToRender(const glm::vec3& playerPosition)
{
	int xRemainder = ((int)playerPosition.x % Chunk::WIDTH);
	if (xRemainder < 0)
		xRemainder += Chunk::WIDTH;
	int zRemainder = ((int)playerPosition.z % Chunk::DEPTH);
	if (zRemainder < 0)
		zRemainder += Chunk::DEPTH;
	int currentChunkX = playerPosition.x - xRemainder;
	int currentChunkZ = playerPosition.z - zRemainder;
	glm::vec3 topLeftChunkPosition = { currentChunkX-(int)RENDER_DISTANCE*Chunk::WIDTH, 0.0, currentChunkZ-(int)RENDER_DISTANCE*Chunk::DEPTH };
	for (unsigned int z=0; z < RENDER_DISTANCE*2+1; z++) {
		for (unsigned int x=0; x < RENDER_DISTANCE*2+1; x++) {
			unsigned int index = x + ((RENDER_DISTANCE*2+1) * z);
			glm::vec3 newPosition = topLeftChunkPosition + glm::vec3{x*Chunk::WIDTH,0.0,z*Chunk::DEPTH};
			m_ChunksToRender[index] = newPosition;
		}
	}
}

void World::LoadChunksInRenderer(Renderer& renderer, ThreadPool& threadPool)
{
	for (const glm::vec3& chunkPosition : m_ChunksToRender) {
		GenerateChunkIfHaveTo(chunkPosition+glm::vec3{0.0,0.0,0.0}, threadPool);
		GenerateChunkIfHaveTo(chunkPosition+glm::vec3{Chunk::WIDTH,0.0,0.0}, threadPool);
		GenerateChunkIfHaveTo(chunkPosition+glm::vec3{-(int)Chunk::WIDTH,0.0,0.0}, threadPool);
		GenerateChunkIfHaveTo(chunkPosition+glm::vec3{0.0,0.0,Chunk::DEPTH}, threadPool);
		GenerateChunkIfHaveTo(chunkPosition+glm::vec3{0.0,0.0,-(int)Chunk::DEPTH}, threadPool);
	}
	for (const glm::vec3& chunkPosition : m_ChunksToRender) {
		LoadChunkIfHaveTo(chunkPosition,threadPool,renderer);
	}
}

void World::LoadChunkIfHaveTo(const glm::vec3& chunkPosition, ThreadPool& threadPool, Renderer& renderer) {
	glm::vec3 rightChunkPosition = chunkPosition + glm::vec3{ Chunk::WIDTH,0.0,0.0 };
	glm::vec3 leftChunkPosition = chunkPosition + glm::vec3{ -(int)Chunk::WIDTH,0.0,0.0 };
	glm::vec3 frontChunkPosition = chunkPosition + glm::vec3{ 0.0,0.0,Chunk::DEPTH };
	glm::vec3 backChunkPosition = chunkPosition + glm::vec3{ 0.0,0.0,-(int)Chunk::DEPTH };
	if (
			(
				m_Chunks.find(chunkPosition) != m_Chunks.end() &&
				m_Chunks.find(rightChunkPosition) != m_Chunks.end() &&
				m_Chunks.find(leftChunkPosition) != m_Chunks.end() &&
				m_Chunks.find(frontChunkPosition) != m_Chunks.end() &&
				m_Chunks.find(backChunkPosition) != m_Chunks.end()
			) &&
		!renderer.IsChunkLoaded(chunkPosition) &&
		!threadPool.IsChunkBeingLoaded(chunkPosition
		)) {
		threadPool.EnqueueChunkLoading(
			m_Chunks[chunkPosition],
			GetChunk(rightChunkPosition),
			GetChunk(leftChunkPosition),
			GetChunk(frontChunkPosition),
			GetChunk(backChunkPosition)
		);
		renderer.WaitForChunkVertices(chunkPosition);
	}
}

void World::GetGeneratedChunksFromThreadLoop(ThreadPool& threadPool)
{
	for (const glm::vec3& chunkOrigin : m_CheckForGeneratedChunks) {
		if (threadPool.HasChunkGenerated(chunkOrigin)) {
			m_Chunks[chunkOrigin] = threadPool.GetGeneratedChunk(chunkOrigin);
		}
	}
}

void World::GenerateChunkIfHaveTo(const glm::vec3& chunkPosition,ThreadPool& threadPool)
{
	if (m_Chunks.find(chunkPosition) == m_Chunks.end() && !threadPool.IsChunkBeingGenerated(chunkPosition)) {
		threadPool.EnqueueChunkGeneration(chunkPosition);
		WaitForChunkGeneration(chunkPosition);
	}
}

Chunk* World::GetChunk(const glm::vec3& chunkOrigin)
{
	if (m_Chunks.find(chunkOrigin) != m_Chunks.end()) {
		return m_Chunks[chunkOrigin];
	}
	return nullptr;
}

void World::WaitForChunkGeneration(glm::vec3 chunkOrigin)
{
	if (std::find(m_CheckForGeneratedChunks.begin(), m_CheckForGeneratedChunks.end(), chunkOrigin)==m_CheckForGeneratedChunks.end()) {
		m_CheckForGeneratedChunks.push_back(chunkOrigin);
	}
}

void World::Draw(const Renderer& renderer)
{
	//std::vector<Vertex> vertices = GetWorldBlocksVertecies();
}

