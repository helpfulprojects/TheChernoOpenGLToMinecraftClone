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
#include "ChunkPositionHash.h"
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
    void EnqueueChunkGeneration(const ChunkPosition& chunkOrigin);
    bool IsChunkBeingLoaded(const ChunkPosition& chunkOrigin);
    bool IsChunkBeingGenerated(const ChunkPosition& chunkOrigin);
    bool HasChunkGenerated(const ChunkPosition& chunkOrigin);
    bool HasChunkLoaded(const ChunkPosition& chunkOrigin);
    std::vector<Vertex> GetChunkTerrainVertices(const ChunkPosition& chunkOrigin);
    std::vector<Vertex> GetChunkWaterVertices(const ChunkPosition& chunkOrigin);
    Chunk* GetGeneratedChunk(const ChunkPosition& chunkOrigin);
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
	std::unordered_map<ChunkPosition, std::vector<Vertex>,ChunkPositionHash> terrainVerticesForRenderer;
	std::mutex terrainVerticesMtx;

	std::unordered_map<ChunkPosition, std::vector<Vertex>,ChunkPositionHash> waterVerticesForRenderer;
	std::mutex waterVerticesMtx;

	std::unordered_map<ChunkPosition, Chunk*, ChunkPositionHash> generatedChunks;
	std::mutex generatedChunksMtx;
};
