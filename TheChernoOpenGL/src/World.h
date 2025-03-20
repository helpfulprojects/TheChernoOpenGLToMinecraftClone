#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <unordered_map>
#include "Chunk.h"
#include <glm/gtx/hash.hpp>
class World {
public:
	~World();
	void UpdateChunksToRender(const glm::vec3& playerPosition);
	void UpdateVertexArrays();
	void Draw(const Renderer& renderer);
private:
	const static unsigned int RENDER_DISTANCE = 1;
	void GenerateChunkIfNotFound(const glm::vec3& chunkPosition);
	glm::vec3 m_ChunksToRender[(RENDER_DISTANCE*2+1)*(RENDER_DISTANCE*2+1)];
	std::unordered_map<glm::vec3,Chunk*> m_Chunks;
};
