#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "RenderBatch.h"
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <vector>
#include "ThreadPool.h"
#include "World.h"

class Renderer {
public:
	~Renderer();
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib) const;
	void Draw(const VertexArray& va) const;
	void DrawTerrain() const;
	void DrawWater() const;
	void AddVertex(const Vertex& vertex,std::vector<RenderBatch*>& renderBatch);
	void AddVertices(const std::vector<Vertex>& vertices, std::vector<RenderBatch*>& renderBatch);
	void LoadChunkTerrain(const ChunkPosition& chunkOrigin, const std::vector<Vertex>& vertices);
	void LoadChunkWater(const ChunkPosition& chunkOrigin, const std::vector<Vertex>& vertices);
	void UnloadChunkTerrain(const ChunkPosition& chunkOrigin);
	void UnloadChunkWater(const ChunkPosition& chunkOrigin);
	bool IsChunkLoaded(const ChunkPosition& chunkOrigin)const;
	void WaitForChunkVertices(const ChunkPosition& chunkOrigin);
	void GetVerticesFromThreadLoop(ThreadPool& threadPool);
	void UnloadChunksTerrain(World& world);
	void UnloadChunksWater(World& world);
private:
	std::unordered_map<ChunkPosition, std::vector<RenderBatch*>, ChunkPositionHash> m_LoadedChunksTerrain;
	std::unordered_map<ChunkPosition, std::vector<RenderBatch*>, ChunkPositionHash> m_LoadedChunksWater;
	std::vector<ChunkPosition> m_CheckForChunkVertices;
};
