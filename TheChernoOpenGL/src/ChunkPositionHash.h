#pragma once
#include "ChunkPosition.h"
#include <unordered_map>

struct ChunkPositionHash
{
	std::size_t operator()(const ChunkPosition& key) const
	{
		std::size_t hx = std::hash<int>()(key.x);
		std::size_t hy = std::hash<int>()(key.y);
		std::size_t hz = std::hash<int>()(key.z);

		// Combine the hashes using bitwise XOR and shifting
		return hx ^ (hy << 1) ^ (hz << 2);
	}
};
