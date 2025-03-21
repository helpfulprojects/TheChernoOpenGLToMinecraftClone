#include "World.h"
#include <cmath>
#include "Renderer.h"

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
		GenerateChunkIfNotFound(chunkPosition+glm::vec3{0.0,0.0,0.0});
		GenerateChunkIfNotFound(chunkPosition+glm::vec3{Chunk::WIDTH,0.0,0.0});
		GenerateChunkIfNotFound(chunkPosition+glm::vec3{-(int)Chunk::WIDTH,0.0,0.0});
		GenerateChunkIfNotFound(chunkPosition+glm::vec3{0.0,0.0,Chunk::DEPTH});
		GenerateChunkIfNotFound(chunkPosition+glm::vec3{0.0,0.0,-(int)Chunk::DEPTH});
	}
	for (const glm::vec3& chunkPosition : m_ChunksToRender) {
		if (m_Chunks.find(chunkPosition) != m_Chunks.end() && !renderer.IsChunkLoaded(chunkPosition) && !threadPool.IsChunkBeingLoaded(chunkPosition)) {
			threadPool.EnqueueChunkLoading(
				m_Chunks[chunkPosition],
				m_Chunks[chunkPosition + glm::vec3{ Chunk::WIDTH,0.0,0.0 }],
				m_Chunks[chunkPosition + glm::vec3{ -(int)Chunk::WIDTH,0.0,0.0 }],
				m_Chunks[chunkPosition + glm::vec3{ 0.0,0.0,Chunk::DEPTH }],
				m_Chunks[chunkPosition + glm::vec3{ 0.0,0.0,-(int)Chunk::DEPTH }]
			);
			renderer.WaitForChunkVertices(chunkPosition);
		}
	}
}

void World::GenerateChunkIfNotFound(const glm::vec3& chunkPosition)
{
	if (m_Chunks.find(chunkPosition) == m_Chunks.end()) {
		m_Chunks[chunkPosition] = new Chunk(chunkPosition);
	}
}

void World::Draw(const Renderer& renderer)
{
	//std::vector<Vertex> vertices = GetWorldBlocksVertecies();
}

