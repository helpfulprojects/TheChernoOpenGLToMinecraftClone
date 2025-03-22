#include "ThreadPool.h"
ThreadPool::ThreadPool(size_t num_threads)
{
	// Creating worker threads
	for (size_t i = 0; i < num_threads; ++i) {
		threads_.emplace_back([this] {
			while (true) {
				std::function<void()> task;
				// The reason for putting the below code
				// here is to unlock the queue before
				// executing the task so that other
				// threads can perform enqueue tasks
				{
					// Locking the queue so that data
					// can be shared safely
					std::unique_lock<std::mutex> lock(
						queue_mutex_);
					// Waiting until there is a task to
					// execute or the pool is stopped
					cv_.wait(lock, [this] {
						return !tasks_.empty() || stop_;
						});
					// exit the thread in case the pool
					// is stopped and there are no tasks
					if (stop_) {
						return;
					}
					// Get the next task from the queue
					task = move(tasks_.front());
					tasks_.pop();
				}
				task();
			}
			});
	}
}
ThreadPool::~ThreadPool()
{
	{
		// Lock the queue to update the stop flag safely
		std::unique_lock<std::mutex> lock(queue_mutex_);
		stop_ = true;
	}
	// Notify all threads
	cv_.notify_all();
	// Joining all worker threads to ensure they have
	// completed their tasks
	for (auto& thread : threads_) {
		thread.join();
	}
}

void ThreadPool::enqueue(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex_);
		tasks_.emplace(move(task));
	}
	cv_.notify_one();
}

void ThreadPool::EnqueueChunkLoading(const Chunk* currentChunk, const Chunk* leftChunk, const Chunk* rightChunk, const Chunk* frontChunk, const Chunk* backChunk)
{
    {
        std::lock_guard<std::mutex> lock(terrainVerticesMtx);
        terrainVerticesForRenderer[currentChunk->GetPosition()] = std::vector<Vertex>(); 
    }
	enqueue([this, currentChunk, leftChunk, rightChunk, frontChunk, backChunk]() {
		std::vector<Vertex> terrainVertices;
		std::vector<Vertex> waterVertices;
		currentChunk->GetChunkBlocksVertecies(terrainVertices,waterVertices,leftChunk, rightChunk, frontChunk, backChunk);
			{
				std::lock_guard<std::mutex> lock(terrainVerticesMtx);
				terrainVerticesForRenderer[currentChunk->GetPosition()] = terrainVertices; 
			}

			{
				std::lock_guard<std::mutex> lock(waterVerticesMtx);
				waterVerticesForRenderer[currentChunk->GetPosition()] = waterVertices; 
			}
		});
}

void ThreadPool::EnqueueChunkGeneration(glm::vec3 chunkOrigin)
{
    {
        std::lock_guard<std::mutex> lock(generatedChunksMtx);
        generatedChunks[chunkOrigin] = nullptr; 
    }
	enqueue([this,chunkOrigin]() {

			Chunk* generatedChunk = new Chunk(chunkOrigin);
			{
				std::lock_guard<std::mutex> lock(generatedChunksMtx);
				generatedChunks[chunkOrigin] = generatedChunk; 
			}
		});
}

bool ThreadPool::IsChunkBeingLoaded(const glm::vec3& chunkOrigin)
{
    return terrainVerticesForRenderer.find(chunkOrigin) != terrainVerticesForRenderer.end();
}

bool ThreadPool::IsChunkBeingGenerated(const glm::vec3& chunkOrigin)
{
	return generatedChunks.find(chunkOrigin) != generatedChunks.end();
}

bool ThreadPool::HasChunkGenerated(const glm::vec3& chunkOrigin)
{
	return IsChunkBeingGenerated(chunkOrigin) && generatedChunks[chunkOrigin];
}

bool ThreadPool::HasChunkLoaded(const glm::vec3& chunkOrigin)
{
	return IsChunkBeingLoaded(chunkOrigin) && terrainVerticesForRenderer[chunkOrigin].size() > 0;
}

std::vector<Vertex> ThreadPool::GetChunkTerrainVertices(const glm::vec3& chunkOrigin)
{
	std::lock_guard<std::mutex> lock(terrainVerticesMtx);
	std::vector<Vertex> vertices = terrainVerticesForRenderer[chunkOrigin];
	terrainVerticesForRenderer.erase(terrainVerticesForRenderer.find(chunkOrigin));
	return vertices;
}

std::vector<Vertex> ThreadPool::GetChunkWaterVertices(const glm::vec3& chunkOrigin)
{
	std::lock_guard<std::mutex> lock(waterVerticesMtx);
	std::vector<Vertex> vertices = waterVerticesForRenderer[chunkOrigin];
	waterVerticesForRenderer.erase(waterVerticesForRenderer.find(chunkOrigin));
	return vertices;
}

Chunk* ThreadPool::GetGeneratedChunk(const glm::vec3& chunkOrigin)
{
	std::lock_guard<std::mutex> lock(generatedChunksMtx);
	Chunk* generatedChunk = generatedChunks[chunkOrigin];
	generatedChunks.erase(generatedChunks.find(chunkOrigin));
	return generatedChunk;
}

