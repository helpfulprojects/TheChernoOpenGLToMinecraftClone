#include "Renderer.h"

Renderer::~Renderer()
{
	for (auto chunkItr = m_LoadedChunks.begin();chunkItr != m_LoadedChunks.end();) {
		UnloadChunk(chunkItr->first);
		chunkItr = m_LoadedChunks.erase(m_LoadedChunks.find(chunkItr->first));
	}
}

void Renderer::Clear() const
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib) const
{
	va.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va) const
{
	va.Bind();
	GlCall(glDrawArrays(GL_TRIANGLES, 0, va.GetVerticesCount()));
}

void Renderer::Draw() const
{
	for (auto renderBatches : m_LoadedChunks) {
		for (RenderBatch* renderBatch : renderBatches.second) {
			renderBatch->Draw();
		}
	}
}

void Renderer::AddVertex(const Vertex& vertex, std::vector<RenderBatch*>& renderBatch)
{
	if (renderBatch.size() <= 0)
		renderBatch.push_back(new RenderBatch());
	RenderBatch* lastRenderBatch = renderBatch.back();
	if (!lastRenderBatch->AddVertex(vertex)) {
		renderBatch.push_back(new RenderBatch());
		AddVertex(vertex,renderBatch);
	}
}

void Renderer::AddVertices(const std::vector<Vertex>& vertices, std::vector<RenderBatch*>& renderBatch)
{
	for (const Vertex& vertex : vertices) {
		AddVertex(vertex,renderBatch);
	}
}

void Renderer::LoadChunk(glm::vec3 chunkOrigin, const std::vector<Vertex>& vertices)
{
	if (m_LoadedChunks.find(chunkOrigin) == m_LoadedChunks.end()) {
		AddVertices(vertices, m_LoadedChunks[chunkOrigin]);
	}
}

void Renderer::UnloadChunk(glm::vec3 chunkOrigin)
{
	if (m_LoadedChunks.find(chunkOrigin) != m_LoadedChunks.end()) {
		for (RenderBatch* renderBatch : m_LoadedChunks[chunkOrigin]) {
			delete renderBatch;
		}
	}
}

bool Renderer::IsChunkLoaded(glm::vec3 chunkOrigin) const
{
	return m_LoadedChunks.find(chunkOrigin) != m_LoadedChunks.end();
}

void Renderer::WaitForChunkVertices(glm::vec3 chunkOrigin)
{
	if (std::find(m_CheckForChunkVertices.begin(), m_CheckForChunkVertices.end(), chunkOrigin)==m_CheckForChunkVertices.end()) {
		m_CheckForChunkVertices.push_back(chunkOrigin);
	}
}

void Renderer::GetVerticesFromThreadLoop(ThreadPool& threadPool)
{
	for (const glm::vec3& chunkOrigin : m_CheckForChunkVertices) {
		if (threadPool.HasChunkLoaded(chunkOrigin)) {
			LoadChunk(chunkOrigin,threadPool.GetChunkVertices(chunkOrigin));
		}
	}
}

void Renderer::UnloadChunks(World& world)
{
	for (auto chunkItr = m_LoadedChunks.begin();chunkItr != m_LoadedChunks.end();) {
		if (std::find(std::begin(world.m_ChunksToRender), std::end(world.m_ChunksToRender), chunkItr->first) == std::end(world.m_ChunksToRender)) {
			UnloadChunk(chunkItr->first);
			chunkItr = m_LoadedChunks.erase(m_LoadedChunks.find(chunkItr->first));
		}
		else {
			chunkItr++;
		}
	}
}
