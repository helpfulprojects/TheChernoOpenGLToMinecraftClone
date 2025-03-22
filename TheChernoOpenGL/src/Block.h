#pragma once
#include <glm/glm.hpp>
enum class BlockType {
	Air = -1,
	Dirt,
	Grass,
	Stone,
	Water
};
struct Vec3 {
	float x;
	float y;
	float z;
	Vec3 operator+(Vec3 const& other) const { return Vec3{ other.x + x,other.y + y,other.z + z }; }
	bool operator==(const Vec3& other) const { return other.x == x && other.y == y && other.z == z; }
};
struct Vec2 {
	float x;
	float y;
	Vec2 operator+(Vec2 const& other) const { return Vec2{ other.x + x,other.y + y}; }
	bool operator==(const Vec2& other) const { return other.x == x && other.y == y; }
};
struct Vertex {
	Vec3 Position;
	Vec2 TexCoords;
	Vec3 Color{1.0,1.0,1.0};
};
