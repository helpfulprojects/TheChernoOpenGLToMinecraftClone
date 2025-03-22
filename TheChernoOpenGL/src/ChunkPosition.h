#pragma once
struct ChunkPosition {
	int x;
	int y;
	int z;
	ChunkPosition(int x, int y, int z) : x(x), y(y), z(z) {}
	ChunkPosition() : ChunkPosition(0,0,0) {}
	ChunkPosition operator+(ChunkPosition const& other) const;
	bool operator==(const ChunkPosition& other) const;
};
