#include "ChunkPosition.h"

ChunkPosition ChunkPosition::operator+(ChunkPosition const& other)const
{
    return ChunkPosition(x+other.x,y+other.y,z+other.z);
}

bool ChunkPosition::operator==(const ChunkPosition& other) const
{
    return other.x == x && other.y == y && other.z == z;
}
