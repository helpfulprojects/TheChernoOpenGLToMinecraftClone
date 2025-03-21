#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "RenderBatch.h"
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <vector>
#include "ThreadPool.h"

class Renderer {
public:
	~Renderer();
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib) const;
	void Draw(const VertexArray& va) const;
	void Draw() const;
	void AddVertex(const Vertex& vertex,std::vector<RenderBatch*>& renderBatch);
	void AddVertices(const std::vector<Vertex>& vertices, std::vector<RenderBatch*>& renderBatch);
	void LoadChunk(glm::vec3 chunkOrigin, const std::vector<Vertex>& vertices);
	void UnloadChunk(glm::vec3 chunkOrigin);
	bool IsChunkLoaded(glm::vec3 chunkOrigin)const;
	void WaitForChunkVertices(glm::vec3 chunkOrigin);
	void GetVerticesFromThreadLoop(ThreadPool& threadPool);
private:
	std::unordered_map<glm::vec3, std::vector<RenderBatch*>> m_LoadedChunks;
	std::vector<glm::vec3> m_CheckForChunkVertices;
};
