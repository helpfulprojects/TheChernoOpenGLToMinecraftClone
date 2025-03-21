#pragma once
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include "Block.h"
#include "Chunk.h"
// Class that represents a simple thread pool
class ThreadPool {
public:
    // // Constructor to creates a thread pool with given
    // number of threads
    ThreadPool(size_t num_threads
        = std::thread::hardware_concurrency());
    // Destructor to stop the thread pool
    ~ThreadPool();
    // Enqueue task for execution by the thread pool
    void enqueue(std::function<void()> task);
    void EnqueueChunkLoading(const Chunk* currentChunk, const Chunk* leftChunk, const Chunk* rightChunk, const Chunk* frontChunk, const Chunk* backChunk);
    void EnqueueChunkGeneration(glm::vec3 chunkOrigin);
    bool IsChunkBeingLoaded(const glm::vec3& chunkOrigin);
    bool IsChunkBeingGenerated(const glm::vec3& chunkOrigin);
    bool HasChunkGenerated(const glm::vec3& chunkOrigin);
    bool HasChunkLoaded(const glm::vec3& chunkOrigin);
    std::vector<Vertex> GetChunkVertices(const glm::vec3& chunkOrigin);
    Chunk* GetGeneratedChunk(const glm::vec3& chunkOrigin);
private:
    // Vector to store worker threads
    std::vector<std::thread> threads_;
    // Queue of tasks
    std::queue<std::function<void()> > tasks_;
    // Mutex to synchronize access to shared data
    std::mutex queue_mutex_;
    // Condition variable to signal changes in the state of
    // the tasks queue
    std::condition_variable cv_;
    // Flag to indicate whether the thread pool should stop
    // or not
    bool stop_ = false;
	std::unordered_map<glm::vec3, std::vector<Vertex>> chunksVerticesForRenderer;
	std::mutex chunksVerticesMtx;

	std::unordered_map<glm::vec3, Chunk*> generatedChunks;
	std::mutex generatedChunksMtx;
};
