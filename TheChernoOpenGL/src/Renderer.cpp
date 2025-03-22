#include "Renderer.h"

Renderer::~Renderer()
{
	for (auto chunkItr = m_LoadedChunksTerrain.begin();chunkItr != m_LoadedChunksTerrain.end();) {
		UnloadChunkTerrain(chunkItr->first);
		chunkItr = m_LoadedChunksTerrain.erase(m_LoadedChunksTerrain.find(chunkItr->first));
	}
	for (auto chunkItr = m_LoadedChunksWater.begin();chunkItr != m_LoadedChunksWater.end();) {
		UnloadChunkWater(chunkItr->first);
		chunkItr = m_LoadedChunksWater.erase(m_LoadedChunksWater.find(chunkItr->first));
	}
}

void Renderer::Clear() const
{
	glClearColor(110.0f/255.0f, 177.0f/255.0f, 255.0f/255.0f, 1.0f);
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

void Renderer::DrawTerrain() const
{
	for (auto renderBatches : m_LoadedChunksTerrain) {
		for (RenderBatch* renderBatch : renderBatches.second) {
			renderBatch->Draw();
		}
	}
}

void Renderer::DrawWater() const
{
	for (auto renderBatches : m_LoadedChunksWater) {
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

void Renderer::LoadChunkTerrain(glm::vec3 chunkOrigin, const std::vector<Vertex>& vertices)
{
	if (m_LoadedChunksTerrain.find(chunkOrigin) == m_LoadedChunksTerrain.end()) {
		AddVertices(vertices, m_LoadedChunksTerrain[chunkOrigin]);
	}
}

void Renderer::LoadChunkWater(glm::vec3 chunkOrigin, const std::vector<Vertex>& vertices)
{
	if (m_LoadedChunksWater.find(chunkOrigin) == m_LoadedChunksWater.end()) {
		AddVertices(vertices, m_LoadedChunksWater[chunkOrigin]);
	}
}

void Renderer::UnloadChunkTerrain(glm::vec3 chunkOrigin)
{
	if (m_LoadedChunksTerrain.find(chunkOrigin) != m_LoadedChunksTerrain.end()) {
		for (RenderBatch* renderBatch : m_LoadedChunksTerrain[chunkOrigin]) {
			delete renderBatch;
		}
	}
}

void Renderer::UnloadChunkWater(glm::vec3 chunkOrigin)
{
	if (m_LoadedChunksWater.find(chunkOrigin) != m_LoadedChunksWater.end()) {
		for (RenderBatch* renderBatch : m_LoadedChunksWater[chunkOrigin]) {
			delete renderBatch;
		}
	}
}

bool Renderer::IsChunkLoaded(glm::vec3 chunkOrigin) const
{
	return m_LoadedChunksTerrain.find(chunkOrigin) != m_LoadedChunksTerrain.end();
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
			LoadChunkTerrain(chunkOrigin,threadPool.GetChunkTerrainVertices(chunkOrigin));
			LoadChunkWater(chunkOrigin,threadPool.GetChunkWaterVertices(chunkOrigin));
		}
	}
}

void Renderer::UnloadChunksTerrain(World& world)
{
	for (auto chunkItr = m_LoadedChunksTerrain.begin();chunkItr != m_LoadedChunksTerrain.end();) {
		if (std::find(std::begin(world.m_ChunksToRender), std::end(world.m_ChunksToRender), chunkItr->first) == std::end(world.m_ChunksToRender)) {
			UnloadChunkTerrain(chunkItr->first);
			chunkItr = m_LoadedChunksTerrain.erase(m_LoadedChunksTerrain.find(chunkItr->first));
		}
		else {
			chunkItr++;
		}
	}
}

void Renderer::UnloadChunksWater(World& world)
{
	for (auto chunkItr = m_LoadedChunksWater.begin();chunkItr != m_LoadedChunksWater.end();) {
		if (std::find(std::begin(world.m_ChunksToRender), std::end(world.m_ChunksToRender), chunkItr->first) == std::end(world.m_ChunksToRender)) {
			UnloadChunkWater(chunkItr->first);
			chunkItr = m_LoadedChunksWater.erase(m_LoadedChunksWater.find(chunkItr->first));
		}
		else {
			chunkItr++;
		}
	}
}
