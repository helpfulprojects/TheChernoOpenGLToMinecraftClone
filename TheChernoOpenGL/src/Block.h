#pragma once
#include <glm/glm.hpp>
enum class BlockType {
	Air = -1,
	Dirt,
	Grass,
	Stone,
	Water
};
struct Vertex {
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Color{1.0,1.0,1.0};
};
